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
#include <os/path.h>
#include <os/vfs.h>
#include <vfs/simple_env.h>

#include "pcap.h"

namespace Fpga {
	using namespace Genode;

	struct Main;
}

struct Fpga::Main
{
	using Type   = Platform::Device::Type;

	Env                               &env;
	Heap                               heap              { env.ram(), env.rm() };

	Attached_rom_dataspace             config_rom        { env, "config" };
	Vfs::Simple_env                    vfs_env           { env, heap, vfs_config() };

	Signal_handler<Main>               config_handler    { env.ep(), *this, &Main::handle_config };
	Constructible<Watch_handler<Main>> bitstream_handler { };

	bool                               enabled           { false };
	Directory::Path                    bitstream_path    { };

	Reporter                           reporter          { env, "state" };

	Platform::Connection               platform          { env };
	Platform::Device                   device            { platform,
	                                                       Type { "xlnx,zynq-devcfg-1.0" } };
	Pcap_loader                        loader            { env, platform, device, reporter };

	Xml_node vfs_config()
	{
		try { return config_rom.xml().sub_node("vfs"); }
		catch (...) {
			Genode::error("VFS not configured");
			env.parent().exit(~0);
			throw;
		}
	}

	void handle_bitstream()
	{
		if (enabled)
			load_bitstream(bitstream_path);
	}

	void handle_config();

	void load_bitstream(Directory::Path const &);

	Main(Env &env) : env(env)
	{
		loader.reset();

		config_rom.sigh(config_handler);

		reporter.enabled(true);

		handle_config();

		/* TODO periodically read back bitstream to check integrity */
	}
};


void Fpga::Main::load_bitstream(Directory::Path const &path)
{
	Directory         root_dir  { vfs_env };
	size_t            file_size { (size_t)root_dir.file_size(path) };

	/* get file name from path using Genode::Path */
	Genode::Path<256> pathname  { path };
	pathname.keep_only_last_element();
	Pcap_loader::Name file_name { &pathname.string()[1] };

	log("Loading bitstream ", path);

	loader.load_bitstream(file_size, file_name, [&] (char * buf) {
		Readonly_file file { root_dir, path };
		return (size_t)file.read(buf, file_size);
	});
}

void Fpga::Main::handle_config()
{
	/* update ROM and apply to vfs */
	config_rom.update();
	vfs_env.root_dir().apply_config(vfs_config());

	/* get enabled state and bitstream path from XML */
	bool            new_enabled = config_rom.xml().attribute_value("enable",    enabled);
	Directory::Path new_path    = config_rom.xml().attribute_value("bitstream", bitstream_path);

	if (enabled && !new_enabled) {
		loader.reset();

		if (bitstream_handler.constructed())
			bitstream_handler.destruct();
	}
	else if (new_enabled) {

		if (new_path != bitstream_path ||
		    new_enabled != enabled)
		{
			/* update watch handler */
			bitstream_handler.conditional(new_enabled,
			                              vfs_env.root_dir(),
			                              new_path,
			                              heap,
			                              *this,
			                              &Main::handle_bitstream);

			load_bitstream(new_path);
		}
	}

	/* store config values */
	enabled        = new_enabled;
	bitstream_path = new_path;
}


void Component::construct(Genode::Env &env) { static Fpga::Main main(env); }
