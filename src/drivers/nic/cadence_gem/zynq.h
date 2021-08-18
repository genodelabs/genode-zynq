/*
 * \author Johannes Schlatow
 * \date   2021-08-18
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__DRIVERS__NIC__CADENCE_GEM__ZYNQ_H_
#define _SRC__DRIVERS__NIC__CADENCE_GEM__ZYNQ_H_

namespace Zynq {
	enum {
		/* needed for Sclr */
		/* TODO remove since Ethernet is already initialized by u-boot */
		MMIO_1_BASE    = 0xF8000000,
		MMIO_1_SIZE    = 0x02000000,

		/* Ethernet 0 */
		EMAC_0_MMIO_BASE   = 0xE000B000,
		EMAC_0_MMIO_SIZE   = 0x1000,
		EMAC_0_IRQ         = 54,

		/* Ethernet 1 */
		EMAC_1_MMIO_BASE   = 0xE000C000,
		EMAC_1_MMIO_SIZE   = 0x1000,
		EMAC_1_IRQ         = 77,
	};
};

#endif /* _SRC__DRIVERS__NIC__CADENCE_GEM__ZYNQ_H_ */
