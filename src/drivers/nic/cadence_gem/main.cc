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
#include <nic/root.h>

/* NIC driver includes */
#include <drivers/nic/mode.h>

/* local includes */
#include "uplink_client.h"
#include "zynq.h"

namespace Server {
	using namespace Genode;

	class Gem_session_component;
	struct Main;
}


Nic::Mac_address
read_mac_addr_from_config(Genode::Attached_rom_dataspace &config_rom)
{
	Nic::Mac_address mac_addr;

	/* fall back to fake MAC address (unicast, locally managed) */
	mac_addr.addr[0] = 0x02;
	mac_addr.addr[1] = 0x00;
	mac_addr.addr[2] = 0x00;
	mac_addr.addr[3] = 0x00;
	mac_addr.addr[4] = 0x00;
	mac_addr.addr[5] = 0x01;

	/* try using configured MAC address */
	try {
		Genode::Xml_node nic_config = config_rom.xml();
		mac_addr = nic_config.attribute_value("mac", mac_addr);
		Genode::log("Using configured MAC address ", mac_addr);
	} catch (...) { }

	return mac_addr;
}


struct Server::Main
{
	Env                                              &_env;
	Heap                                              _heap          { _env.ram(), _env.rm() };

	/* TODO remove Constructible */
	Constructible<Cadence_gem::Device>                _device        { };
	Constructible<Cadence_gem::Uplink_client>         _uplink_client { };

	Main(Env &env) : _env(env)
	{
		Attached_rom_dataspace config_rom { _env, "config" };

		/* TODO get MMIO address from platform session */

		const unsigned interface = config_rom.xml().attribute_value("interface", (unsigned)0);

		if (interface > 1)
			error("Invalid interface: ", interface, ". Falling back to default interface 0");

		const addr_t mmio_base = (interface == 1) ? Zynq::EMAC_1_MMIO_BASE : Zynq::EMAC_0_MMIO_BASE;
		const size_t mmio_size = (interface == 1) ? Zynq::EMAC_1_MMIO_SIZE : Zynq::EMAC_0_MMIO_SIZE;
		const int    irq       = (interface == 1) ? Zynq::EMAC_1_IRQ       : Zynq::EMAC_0_IRQ;

		_device.construct(_env, mmio_base, mmio_size, irq);

		/* TODO don't set MAC if not configured (let u-boot do that) */
		_uplink_client.construct(_env, _heap, *_device,
		                         read_mac_addr_from_config(config_rom));
	}
};


void Component::construct(Genode::Env &env) { static Server::Main main(env); }
