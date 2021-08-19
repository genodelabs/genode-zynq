/*
 * \brief  Board driver for core on Zynq
 * \author Johannes Schlatow
 * \author Stefan Kalkowski
 * \author Martin Stein
 * \date   2014-06-02
 */

/*
 * Copyright (C) 2014-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__BOARD__ZYNQ_ZYBO_Z7__BOARD_H_
#define _CORE__BOARD__ZYNQ_ZYBO_Z7__BOARD_H_

/* core includes */
#include <hw/spec/arm/zynq_zybo_z7_board.h>
#include <drivers/uart/xilinx.h>

#include <hw/spec/arm/boot_info.h>
#include <hw/spec/arm/cortex_a9.h>
#include <hw/spec/arm/pl310.h>
#include <spec/arm/cortex_a9_private_timer.h>

namespace Board {
	using namespace Hw::Zynq_zybo_z7_board;
	using L2_cache = Hw::Pl310;

	L2_cache & l2_cache();
}

#endif /* _CORE__BOARD__ZYNQ_ZYBO_Z7__BOARD_H_ */
