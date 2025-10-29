/*
 * \brief   Zynq specific board definitions
 * \author  Stefan Kalkowski
 * \date    2017-02-20
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__BOOTSTRAP__BOARD__ZYNQ_USRP_E31x__BOARD_H_
#define _SRC__BOOTSTRAP__BOARD__ZYNQ_USRP_E31x__BOARD_H_

#include <hw/spec/arm/zynq_usrp_e31x_board.h>
#include <spec/arm/cortex_a9_actlr.h>
#include <spec/arm/cortex_a9_page_table.h>
#include <spec/arm/cpu.h>
#include <hw/spec/arm/gicv2.h>

namespace Board {
	using namespace Hw::Zynq_usrp_e31x_board;
	using Hw::Global_interrupt_controller;
	using Hw::Local_interrupt_controller;

	static constexpr bool NON_SECURE = false;
}

#endif /* _SRC__BOOTSTRAP__BOARD__ZYNQ_USRP_E31x__BOARD_H_ */
