/*
 * \brief  Driver-specific emulation of Linux Kernel functions
 * \author Johannes Schlatow
 * \date   2022-04-08
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#include <linux/fs.h>

int alloc_chrdev_region(dev_t * dev,unsigned baseminor,unsigned count,const char * name)
{
	static dev_t counter = 0;
	*dev = counter++;
	return 0;
}


#include <asm/delay.h>

void __dummy_delay(unsigned long v) {
	lx_emul_trace_and_stop(__func__);
}

void __dummy_udelay(unsigned long v) {
	lx_emul_trace_and_stop(__func__);
}

/* declaration for lib/lx_emul/zynq/time.cc */
void lx_emul_time_udelay(unsigned long usec);

void lx_emul_const_udelay(unsigned long xloop)
{
	lx_emul_time_udelay(xloop / UDELAY_MULT);
}

struct arm_delay_ops arm_delay_ops = {
	.delay        = __dummy_delay,
	.const_udelay = lx_emul_const_udelay,
	.udelay       = __dummy_udelay,
};
