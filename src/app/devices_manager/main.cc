/*
 * \brief  Reports devices ROM depending on loaded bitstreams
 * \author Johannes Schlatow
 * \date   2022-09-16
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/component.h>
#include <base/attached_rom_dataspace.h>
#include <os/reporter.h>
#include <log_session/log_session.h>

namespace Devices_manager {
	using namespace Genode;

	struct Main;
}


struct Devices_manager::Main
{
	using Bitstream_name = Genode::String<256>;

	Env                   &_env;

	Attached_rom_dataspace _config           { _env, "config" };
	Attached_rom_dataspace _devices          { _env, "devices" };
	Attached_rom_dataspace _fpga_state       { _env, "fpga_state" };

	Expanding_reporter     _devices_reporter { _env, "devices", "devices" };

	Signal_handler<Main> _update_handler {
		_env.ep(), *this, &Main::_handle_update };

	template <typename FN>
	void _with_bitstream_devices(Bitstream_name const &, FN &&);

	void _handle_update();

	Main(Env &env) : _env(env)
	{
		_config    .sigh(_update_handler);
		_devices   .sigh(_update_handler);
		_fpga_state.sigh(_update_handler);

		_handle_update();
	}
};


template <typename FN>
void Devices_manager::Main::_with_bitstream_devices(Bitstream_name const &name, FN && fn)
{
	bool found = false;

	_config.xml().for_each_sub_node("bitstream", [&] (Xml_node const &node) {
		if (found) return;

		Bitstream_name cur_name = node.attribute_value("name", Bitstream_name { });
		if (cur_name == name) {
			node.with_optional_sub_node("devices", [&] (Xml_node const &devices) {
				fn(devices);
			});
			found = true;
		}
	});

	if (!found)
		warning("no entry found for bitstream '", name, "'");
}


void Devices_manager::Main::_handle_update()
{
	_config    .update();
	_devices   .update();
	_fpga_state.update();

	_devices_reporter.generate([&] (Xml_generator &xml) {
		auto copy_xml_content = [&] (char const *start, size_t len) { xml.append(start, len); };

		/* copy devices rom */
		_devices.xml().with_raw_content(copy_xml_content);

		if (!_fpga_state.xml().has_type("state")) return;

		/* copy devices of currently loaded bitstream */
		_fpga_state.xml().with_optional_sub_node("bitstream", [&] (Xml_node const &node) {
			Bitstream_name name = node.attribute_value("name", Bitstream_name { });

			if (node.attribute_value("loaded", false))
				_with_bitstream_devices(name, [&] (Xml_node const &devices) {
					devices.with_raw_content(copy_xml_content);
				});
		});
	});
}


void Component::construct(Genode::Env &env) { static Devices_manager::Main main(env); }
