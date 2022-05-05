/*
 * \brief  Platform driver for Zynq
 * \author Johannes Schlatow
 * \date   2022-04-05
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/component.h>

#include <slcr.h>
#include <root.h>

namespace Driver { struct Main; };

struct Driver::Main
{
	Env                  & _env;
	Heap                   _heap        { _env.ram(), _env.rm() };
	Sliced_heap            _sliced_heap { _env.ram(), _env.rm() };
	Attached_rom_dataspace _config      { _env, "config" };
	Device_model           _devices     { _heap };

	/**
	 * PS_CLK is either 33.33333Mhz or 50MHz. The latter is very rare though
	 * and practically not in use, hence we stick to 33.33333Mhz.
	 */
	Fixed_clock _ps_clk { _devices.clocks(), "ps_clk",
	                      Clock::Rate { 33333333 } };

	Slcr  _slcr { _env, _devices.clocks(), _ps_clk };

	void _handle_config();

	Signal_handler<Main> _config_handler { _env.ep(), *this,
	                                       &Main::_handle_config };

	Driver::Root _root { _env, _sliced_heap, _config, _devices };

	Main(Genode::Env &env) : _env(env)
	{
		_devices.update(_config.xml());
		_config.sigh(_config_handler);
		_env.parent().announce(_env.ep().manage(_root));
	}
};


void Driver::Main::_handle_config()
{
	_config.update();
	_devices.update(_config.xml());
	_root.update_policy();
}


void Component::construct(Genode::Env &env) { static Driver::Main main(env); }
