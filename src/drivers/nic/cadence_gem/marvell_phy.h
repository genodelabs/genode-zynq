/*
 * \brief  Phy driver for Marvell chips
 * \author Johannes Schlatow
 * \author Timo Wischer
 * \date   2015-05-11
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__DRIVERS__NIC__CADENCE_GEM__MARVELL_PHY_H_
#define _INCLUDE__DRIVERS__NIC__CADENCE_GEM__MARVELL_PHY_H_

/* Genode includes */
#include <os/attached_mmio.h>
#include <nic_session/nic_session.h>

#include "phyio.h"

namespace Genode
{

	enum Eth_speed {
		SPEED_UNDEFINED,
		SPEED_10 = 10,
		SPEED_100 = 100,
		SPEED_1000 = 1000
	};

	class MII_phy
	{
		protected:
			/**
			 * \param _OFFSET        Offset/number of the register.
			 *
			 * For further details See 'Genode::Register'.
			 */
			template <uint8_t _OFFSET>
			struct Phy_register : public Genode::Register<16>
			{
				enum {
					OFFSET       = _OFFSET,
				};

				typedef Phy_register<_OFFSET>
					Register_base;

				/**
				 * A region within a register
				 *
				 * \param _SHIFT  Bit shift of the first bit within the
				 *                compound register.
				 * \param _WIDTH  bit width of the region
				 *
				 * For details see 'Genode::Register::Bitfield'.
				 */
				template <unsigned long _SHIFT, unsigned long _WIDTH>
				struct Bitfield : public Genode::Register<16>::template Bitfield<_SHIFT, _WIDTH>
				{
					typedef Bitfield<_SHIFT, _WIDTH> Bitfield_base;

					/* back reference to containing register */
					typedef Phy_register<_OFFSET>
						Compound_reg;
				};
			};

			/*************************
			 * Generic MII registers *
			 *************************/

			/* Basic mode control register */
			struct Bmcr : Phy_register<0x00>
			{
				struct Resv      : Bitfield<0, 6> { }; /* Unused...		       */
				struct Speed1000 : Bitfield<6, 1> { }; /* MSB of Speed (1000)	       */
				struct Ctst      : Bitfield<7, 1> { }; /* Collision test	       */
				struct Fulldplx  : Bitfield<8, 1> { }; /* Full duplex		       */
				struct Anrestart : Bitfield<9, 1> { }; /* Auto negotiation restart    */
				struct Isolate   : Bitfield<10,1> { }; /* Disconnect DP83840 from MII */
				struct Pdown     : Bitfield<11,1> { }; /* Powerdown the DP83840       */
				struct Anenable  : Bitfield<12,1> { }; /* Enable auto negotiation     */
				struct Speed100  : Bitfield<13,1> { }; /* Select 100Mbps	       */
				struct Loopback  : Bitfield<14,1> { }; /* TXD loopback bits	       */
				struct Reset     : Bitfield<15,1> { }; /* Reset the DP83840	       */
			};

			/* Basic mode status register */
			struct Bmsr : Phy_register<0x01>
			{
				enum {
					INVALID        = 0xFFFF
				};

				struct Ercap       : Bitfield<0, 1> { }; /* Ext-reg capability      */
				struct Jcd         : Bitfield<1, 1> { }; /* Jabber detected         */
				struct Lstatus     : Bitfield<2, 1> { }; /* Link status             */
				struct Anegcapable : Bitfield<3, 1> { }; /* Able to do auto-negotiation */
				struct Rfault      : Bitfield<4, 1> { }; /* Remote fault detected       */
				struct Anegcomplete: Bitfield<5, 1> { }; /* Auto-negotiation complete   */
				struct Resv        : Bitfield<6, 1> { }; /* Unused...		       */
				struct Estaten     : Bitfield<7, 1> { }; /* Extended Status in R15 */
				struct Half2_100   : Bitfield<8, 1> { }; /* Can do 100BASE-T2 HDX */
				struct Full2_100   : Bitfield<9, 1> { }; /* Can do 100BASE-T2 FDX */
				struct Half_10     : Bitfield<10,1> { }; /* Can do 10mbps, half-duplex  */
				struct Full_10     : Bitfield<11,1> { }; /* Can do 10mbps, full-duplex  */
				struct Half_100    : Bitfield<12,1> { }; /* Can do 100mbps, half-duplex */
				struct Full_100    : Bitfield<13,1> { }; /* Can do 100mbps, full-duplex */
				struct Base4_100   : Bitfield<14,1> { }; /* Can do 100mbps, 4k packets  */
			};

			/* ID register 1 */
			struct Idr1 : Phy_register<0x02> { };

			/* ID register 2 */
			struct Idr2 : Phy_register<0x03> { };

			/* Advertisement control reg   */
			struct Advertise : Phy_register<0x04>
			{
				struct Csma         : Bitfield<0, 1> { };
				struct Half_10      : Bitfield<5, 1> { }; /* Try for 10mbps half-duplex  */
				struct FullX_1000   : Bitfield<5, 1> { }; /* Try for 1000BASE-X full-duplex */
				struct Full_10      : Bitfield<6, 1> { }; /* Try for 10mbps full-duplex  */
				struct HalfX_1000   : Bitfield<6, 1> { }; /* Try for 1000BASE-X half-duplex */
				struct Half_100     : Bitfield<7, 1> { }; /* Try for 100mbps half-duplex  */
				struct PauseX_1000  : Bitfield<7, 1> { }; /* Try for 1000BASE-X pause */
				struct Full_100     : Bitfield<8, 1> { }; /* Try for 100mbps full-duplex  */
				struct AsymXPSE_1000: Bitfield<8, 1> { }; /* Try for 1000BASE-X asym pause */
				struct Base4_100    : Bitfield<9, 1> { }; /* Try for 100mbps 4k packets  */
				struct Pause_cap    : Bitfield<10,1> { }; /* Try for pause	       */
				struct Pause_asym   : Bitfield<11,1> { }; /* Try for asymetrict pause */
				struct Rfault       : Bitfield<13,1> { }; /* Say we can detect faults    */
				struct Lpack        : Bitfield<14,1> { }; /* Ack link partners response  */
				struct Npage        : Bitfield<15,1> { }; /* Next page bit	       */
			};

			/* 1000BASE-T control	       */
			struct Ctrl1000 : Phy_register<0x09>
			{
				struct Half_1000 : Bitfield<8, 1> { }; /* Advertise 1000BASE-T full duplex */
				struct Full_1000 : Bitfield<9, 1> { }; /* Advertise 1000BASE-T half duplex */
			}; 
	};

	class Marvel_phy : public MII_phy
	{
		public:
			class Phy_timeout_after_reset : public Genode::Exception {};


		private:

			Phyio& _phyio;
			int8_t _phyaddr;


			/*************************
			 * 88E1310 PHY registers *
			 *************************/

			struct Led_ctrl : Phy_register<16>
			{
				struct Data : Bitfield<0, 4> { };
			};

			struct Irq_en : Phy_register<18> { };

			struct RGMII_ctrl : Phy_register<21> { };

			struct Page_select : Phy_register<22> { };

			/* 88E1011 PHY Status Register */
			struct Phy_stat : Phy_register<0x11>
			{
				struct Link        : Bitfield<10,1> { };
				struct Spddone     : Bitfield<11,1> { };
				struct Duplex      : Bitfield<13,1> { };
				struct Speed_100   : Bitfield<14,1> { };
				struct Speed_1000  : Bitfield<15,1> { };
			};

			/**
			 * Read register of detected PHY.
			 */
			inline uint16_t _phy_read(const uint8_t regnum) const
			{
				uint16_t val;
				_phyio.phy_read(_phyaddr, regnum, val);
				return val;
			}

			/**
			 * Write register of detected PHY.
			 */
			inline void _phy_write(const uint8_t regnum, const uint16_t data) const
			{
				_phyio.phy_write(_phyaddr, regnum, data);
			}

			/**
			 * Read PHY register 'T'
			 */
			template <typename T>
			inline typename T::Register_base::access_t phy_read() const
			{
				typedef typename T::Register_base Register;
				return _phy_read(Register::OFFSET);
			}

			/**
			 * Read the bitfield 'T' of PHY register
			 */
			template <typename T>
			inline typename T::Bitfield_base::Compound_reg::access_t
			phy_read() const
			{
				typedef typename T::Bitfield_base Bitfield;
				typedef typename Bitfield::Compound_reg Register;
				return Bitfield::get(_phy_read(Register::OFFSET));
			}

			/**
			 * Write PHY register 'T'
			 */
			template <typename T>
			inline void phy_write(uint16_t const val) const
			{
				typedef typename T::Register_base Register;
				return _phy_write(Register::OFFSET, val);
			}

		public:

			Marvel_phy(Phyio& phyio)
			:
				_phyio(phyio),
				_phyaddr(0)
			{ }

			Eth_speed eth_speed()
			{
				Phy_stat::access_t stat = phy_read<Phy_stat>();

				if (!Phy_stat::Link::get(stat))
					return SPEED_UNDEFINED;

				if (!Phy_stat::Spddone::get(stat))
					return SPEED_UNDEFINED;

				if (Phy_stat::Speed_1000::get(stat))
					return SPEED_1000;
				else if (Phy_stat::Speed_100::get(stat))
					return SPEED_100;
				else
					return SPEED_10;

				return SPEED_UNDEFINED;
			}

	};
}

#endif /* _INCLUDE__DRIVERS__NIC__CADENCE_GEM__MARVELL_PHY_H_ */

