/*
 * \brief  MMIO and IRQ definitions common to Xilinx Zynq platforms
 * \author Mark Albers
 * \author Timo Wischer
 * \author Johannes Schlatow
 * \date   2014-12-15
 */

/*
 * Copyright (C) 2014-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__INCLUDE__HW__SPEC__ARM__ZYNQ_H_
#define _SRC__INCLUDE__HW__SPEC__ARM__ZYNQ_H_

namespace Zynq {
	enum {
		/* device IO memory */
		MMIO_0_BASE    = 0xe0000000, /* IOP devices */
		MMIO_0_SIZE    = 0x10000000,

		/* normal RAM */
		RAM_0_BASE = 0x00000000,

		/* UART controllers */
		UART_0_MMIO_BASE = MMIO_0_BASE,
		UART_SIZE        = 0x1000,
		UART_CLOCK       = 50*1000*1000,

		/* CPU */
		CORTEX_A9_PRIVATE_MEM_BASE  = 0xf8f00000,
		CORTEX_A9_PRIVATE_MEM_SIZE  = 0x00002000,

		/* entrypoint address of secondary cpu */
		CORE1_ENTRY = 0xfffffff0,

		/* CPU cache */
		PL310_MMIO_BASE      = 0xF8F02000,
		PL310_MMIO_SIZE      = 0x1000,
	};
};

#endif /* _SRC__INCLUDE__HW__SPEC__ARM__ZYNQ_H_ */
