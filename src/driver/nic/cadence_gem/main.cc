/*
 * \brief  EMACPS NIC driver for Xilix Zynq-7000
 * \author Timo Wischer
 * \date   2015-03-10
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <base/heap.h>

/* local includes */
#include "uplink_client.h"
#include "zynq.h"

namespace Server {
	using namespace Genode;

	class Gem_session_component;
	struct Main;
}


struct Server::Main
{
	Env                       &_env;
	Heap                       _heap          { _env.ram(), _env.rm() };
	Platform::Connection       _platform      { _env };
	Platform::Device           _pfdevice      { _platform };
	Cadence_gem::Device        _device        { _env, _pfdevice };
	Cadence_gem::Uplink_client _uplink_client { _env, _heap, _device,
	                                            _platform, _mac_addr() };

	Nic::Mac_address _mac_addr()
	{
		Attached_rom_dataspace config_rom { _env, "config" };

		/* read MAC address from config or take from device as fallback */
		Nic::Mac_address mac_addr = _device.read_mac_address();
		Node const &config = config_rom.node();
		return config.attribute_value("mac", mac_addr);
	}

	Main(Env &env) : _env(env) { }
};


void Component::construct(Genode::Env &env) { static Server::Main main(env); }
