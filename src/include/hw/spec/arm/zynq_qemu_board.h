/*
 * \brief   Zynq specific board definitions
 * \author  Stefan Kalkowski
 * \date    2019-05-16
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__INCLUDE__HW__SPEC__ARM__ZYNQ_BOARD_H_
#define _SRC__INCLUDE__HW__SPEC__ARM__ZYNQ_BOARD_H_

#include <hw/spec/arm/zynq_qemu.h>
#include <drivers/uart/xilinx.h>
#include <hw/spec/arm/boot_info.h>

namespace Hw::Zynq_qemu_board {

	using namespace Zynq_qemu;
	using Serial   = Genode::Xilinx_uart;

	static constexpr Genode::size_t NR_OF_CPUS = 1;
}

#endif /* _SRC__INCLUDE__HW__SPEC__ARM__ZYNQ_BOARD_H_ */
