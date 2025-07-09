/*
 * \brief  FPGA driver for initialisation and configuration
 * \author Johannes Schlatow
 * \date   2021-11-11
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/component.h>
#include <base/heap.h>
#include <base/attached_rom_dataspace.h>
#include <util/reconstructible.h>
#include <os/reporter.h>

#include "pcap.h"
#include "bitstream.h"

namespace Fpga {
	using namespace Genode;

	struct Managed_bitstream;
	struct Main;
}

struct Fpga::Managed_bitstream
{
	using Name = String<128>;

	size_t                    max_size;
	Name                      name;
	Attached_rom_dataspace    rom;
	Constructible<Bitstream>  bitstream { };
	Pcap_loader              &loader;
	Expanding_reporter       &reporter;

	Signal_handler<Managed_bitstream> rom_handler;

	Managed_bitstream(Env &env, Pcap_loader &loader, Expanding_reporter &reporter,
	                  Name const &name, size_t max_size)
	: max_size(max_size),
	  name(name),
	  rom(env, name.string()),
	  loader(loader),
	  reporter(reporter),
	  rom_handler(env.ep(), *this, &Managed_bitstream::handle_rom)
	{
		handle_rom();

		rom.sigh(rom_handler);
	}

	~Managed_bitstream()
	{
		/* report that we unload the bitstream */
		report("", false);

		loader.reset();
	}

	void handle_rom()
	{
		rom.update();

		bitstream.construct(rom, max_size);

		log("Loading bitstream ", name, " of size ", Hex(bitstream->size()));

		bool loaded = loader.load_bitstream(bitstream->size(), [&] (char * buf, size_t buf_sz) {
			return bitstream->read_bitstream(buf, buf_sz);
		});

		report(name, loaded);
	}

	void report(Name const &name, bool loaded)
	{
		reporter.generate([&] (Generator &g) {
			g.node("bitstream", [&] () {
				if (name != "")
					g.attribute("name", name);
				g.attribute("loaded", loaded);
			});
		});
	}

};

struct Fpga::Main
{
	using Type   = Platform::Device::Type;

	Env                               &env;
	Heap                               heap              { env.ram(), env.rm() };

	Attached_rom_dataspace             config_rom        { env, "config" };

	Signal_handler<Main>               config_handler    { env.ep(), *this, &Main::handle_config };

	Constructible<Managed_bitstream>   managed_bitstream { };

	Expanding_reporter                 reporter          { env, "state" };

	Platform::Connection               platform          { env };
	Pcap_loader                        loader            { env, platform };

	void handle_config();

	Main(Env &env) : env(env)
	{
		loader.reset();

		config_rom.sigh(config_handler);

		handle_config();

		/* TODO periodically read back bitstream to check integrity */
	}
};


void Fpga::Main::handle_config()
{
	config_rom.update();

	config_rom.node().with_sub_node("bitstream",
		[&] (Node const &g) {
			Managed_bitstream::Name new_name = g.attribute_value("name", Managed_bitstream::Name(""));
			size_t                  max_size = g.attribute_value("size", 0);

			if (new_name == "")
				managed_bitstream.destruct();
			else if (!managed_bitstream.constructed() || new_name != managed_bitstream->name)
				managed_bitstream.construct(env, loader, reporter, new_name, max_size);

		},
		[&] () {
			warning("<bitstream> missing");
			managed_bitstream.destruct();
		}
	);
}


void Component::construct(Genode::Env &env) { static Fpga::Main main(env); }
