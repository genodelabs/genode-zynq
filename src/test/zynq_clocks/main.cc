/*
 * \brief  Test for Zynq platform driver clocks
 * \author Johannes Schlatow
 * \date   2022-04-05
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/component.h>
#include <platform_session/connection.h>

using namespace Genode;

struct Main
{
	using Name = String<32>;

	Env                 &env;
	Platform::Connection platform  { env };

	Main(Env &env) : env(env)
	{
		log("--- Zynq Clock Test ---");
		platform.update();
		platform.with_node([&] (Node const &node) {
			node.for_each_sub_node("device", [&] (Node const &node) {
				log("Device \"", node.attribute_value("name", Name()), "\":");

				node.for_each_sub_node("clock", [&] (Node const &node) {
					log("  Clock \"", node.attribute_value("name", Name()), "\": \t",
					                  node.attribute_value("rate", 0U));
				});
			});
		});

	}
};

void Component::construct(Env &env) { static Main main(env); }
