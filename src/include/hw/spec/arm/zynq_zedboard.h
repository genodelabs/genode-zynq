/*
 * \brief  Base driver for Zedboard
 * \author Mark Albers
 * \date   2015-09-29
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__INCLUDE__HW__SPEC__ARM__ZYNQ_ZEDBOARD_H_
#define _SRC__INCLUDE__HW__SPEC__ARM__ZYNQ_ZEDBOARD_H_

#include <hw/spec/arm/zynq.h>

namespace Zynq_zedboard {

	using namespace Zynq;

	enum {
		/* clocks (assuming 6:2:1 mode) */
		CPU_1X_CLOCK = 111111115,
		CPU_6X4X_CLOCK = 6*CPU_1X_CLOCK,
		CPU_3X2X_CLOCK = 3*CPU_1X_CLOCK,
		CPU_2X_CLOCK = 2*CPU_1X_CLOCK,

		RAM_0_SIZE = 0x20000000, /* 512MB */

		CORTEX_A9_CLOCK             = CPU_6X4X_CLOCK,
		CORTEX_A9_PRIVATE_TIMER_CLK = CPU_3X2X_CLOCK,
		CORTEX_A9_PRIVATE_TIMER_DIV = 100,
	};
};

#endif /* _SRC__INCLUDE__HW__SPEC__ARM__ZYNQ_ZEDBOARD_H_ */
