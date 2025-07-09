/*
 * \brief  Driver manager for Zynq (loads user-defined drivers after FPGA reconfiguration)
 * \author Johannes Schlatow
 * \date   2021-11-15
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/component.h>
#include <base/registry.h>
#include <base/heap.h>
#include <base/attached_rom_dataspace.h>
#include <os/reporter.h>
#include <rm_session/rm_session.h>
#include <io_mem_session/io_mem_session.h>
#include <timer_session/timer_session.h>
#include <log_session/log_session.h>
#include <platform_session/platform_session.h>
#include <nic_session/nic_session.h>
#include <uplink_session/uplink_session.h>
#include <pin_control_session/pin_control_session.h>
#include <pin_state_session/pin_state_session.h>
#include <irq_session/irq_session.h>

namespace Driver_manager {
	using namespace Genode;
	struct Main;
	struct User_logic_driver;
}


struct Driver_manager::User_logic_driver : Registry<User_logic_driver>::Element
{
	using Name = String<64>;

	Buffered_node const driver_node;

	bool start { false };

	static void _gen_forwarded_service(Generator  &g,
	                                   Name const &service,
	                                   Name const &child)
	{
		g.node("service", [&] () {
			g.attribute("name", service);
			g.node("default-policy", [&] () {
				g.node("child", [&] () {
					g.attribute("name", child);
				});
			});
		});
	}

	void generate_start_node(Generator &g) const
	{
		if (!start) return;

		Name const name = driver_node.attribute_value("name", Name());

		driver_node.with_optional_sub_node("provides", [&] (Node const &provides) {

			/* generate forward rules for every provided service */
			provides.for_each_sub_node("service", [&](Node const &service) {
				_gen_forwarded_service(g,
				                       service.attribute_value("name", Name()),
				                       name);
			});
		});

		/* copy content from driver_node */
		g.node("start", [&] () {
			g.attribute("name", name);
			(void)g.append_node_content(driver_node, Generator::Max_depth { 10 });
		});
	}

	User_logic_driver(Registry<User_logic_driver> &registry,
	                  Allocator &alloc, Node const &node)
	:
		Registry<User_logic_driver>::Element(registry, *this),
		driver_node(alloc, node)
	{ }
};


struct Driver_manager::Main
{
	Env                   &_env;
	Heap                   _heap    { _env.ram(), _env.rm() };

	Attached_rom_dataspace _devices { _env, "devices" };
	Attached_rom_dataspace _config  { _env, "config" };

	Reporter _init_config    { _env, "config", "init.config", 8192 };

	Registry<User_logic_driver> _user_logic_drivers { };

	void _handle_devices_update();

	Signal_handler<Main> _devices_update_handler {
		_env.ep(), *this, &Main::_handle_devices_update };

	static void _gen_parent_service(Generator &g, char const *name)
	{
		g.node("service", [&] () { g.attribute("name", name); });
	};

	void _apply_config();

	void _generate_init_config    (Reporter &) const;

	Main(Env &env) : _env(env)
	{
		_init_config.enabled(true);

		_devices.sigh(_devices_update_handler);

		_apply_config();

		_generate_init_config(_init_config);

		_handle_devices_update();
	}
};


void Driver_manager::Main::_apply_config()
{
	_config.update();

	if (!_config.valid())
		return;

	/* clear registry */
	_user_logic_drivers.for_each([&] (User_logic_driver &d) { destroy(_heap, &d); });

	/* fill registry from config */
	_config.node().for_each_sub_node("driver", [&] (Node const &driver) {
		if (!driver.has_attribute("device"))
			return;
		if (!driver.has_attribute("name"))
			return;

		new (_heap) User_logic_driver(_user_logic_drivers, _heap, driver);
	});
}


void Driver_manager::Main::_handle_devices_update()
{
	_devices.update();

	if (!_devices.valid())
		return;

	bool changed = false;

	_user_logic_drivers.for_each([&] (User_logic_driver &d) {

		bool found = false;
		_devices.node().for_each_sub_node([&] (Node const &device) {
			if (device       .attribute_value("type",   User_logic_driver::Name()) ==
			    d.driver_node.attribute_value("device", User_logic_driver::Name()))
			{
				changed = changed || !d.start;
				d.start = true;
				found = true;
			}
		});

		if (!found) {
			changed = changed || d.start;
			d.start = false;
		}

	});

	if (changed)
		_generate_init_config(_init_config);

}


void Driver_manager::Main::_generate_init_config(Reporter &init_config) const
{
	Reporter::Result const result = init_config.generate([&] (Generator &g) {

		g.attribute("verbose", false);

		g.node("heartbeat", [&] () { g.attribute("rate_ms", 2500); });

		g.node("parent-provides", [&] () {
			_gen_parent_service(g, Rom_session::service_name());
			_gen_parent_service(g, Io_mem_session::service_name());
			_gen_parent_service(g, Cpu_session::service_name());
			_gen_parent_service(g, Pd_session::service_name());
			_gen_parent_service(g, Rm_session::service_name());
			_gen_parent_service(g, Log_session::service_name());
			_gen_parent_service(g, Timer::Session::service_name());
			_gen_parent_service(g, Platform::Session::service_name());
			_gen_parent_service(g, Report::Session::service_name());
			_gen_parent_service(g, Uplink::Session::service_name());
			_gen_parent_service(g, Nic::Session::service_name());
			_gen_parent_service(g, Pin_state::Session::service_name());
			_gen_parent_service(g, Pin_control::Session::service_name());
			_gen_parent_service(g, Irq_session::service_name());
		});

		g.node("default-route", [&] () {
			g.node("any-service", [&] () {
				g.node("parent", [&] () { });
			});
		});

		g.node("default", [&] () {
			g.attribute("caps", 300);
		});

		_user_logic_drivers.for_each([&] (User_logic_driver const &d) {
			d.generate_start_node(g);
		});
	});

	if (result == Buffer_error::EXCEEDED)
		warning("init config report exceeds maximum size");
}


void Component::construct(Genode::Env &env) { static Driver_manager::Main main(env); }
