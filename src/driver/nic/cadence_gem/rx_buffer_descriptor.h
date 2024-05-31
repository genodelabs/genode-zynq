/*
 * \brief  Base EMAC driver for the Xilinx EMAC PS used on Zynq devices
 * \author Timo Wischer
 * \author Johannes Schlatow
 * \date   2015-03-10
 */

/*
 * Copyright (C) 2015-2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__DRIVERS__NIC__CADENCE_GEM__RX_BUFFER_DESCRIPTOR_H_
#define _INCLUDE__DRIVERS__NIC__CADENCE_GEM__RX_BUFFER_DESCRIPTOR_H_

#include <nic_session/nic_session.h>
#include "buffer_descriptor.h"

namespace Cadence_gem {
	using namespace Genode;

	template <typename SOURCE, typename DMA_POOL>
	class Rx_buffer_descriptor;
}

template <typename SOURCE, typename DMA_POOL>
class Cadence_gem::Rx_buffer_descriptor : public Buffer_descriptor
{
	private:

		struct Addr : Register<0x00, 32> {
			struct Addr31to2 : Bitfield<2, 30> {};
			struct Wrap : Bitfield<1, 1> {};
			struct Used : Bitfield<0, 1> {};
		};
		struct Status : Register<0x04, 32> {
			struct Length : Bitfield<0, 13> {};
			struct Start_of_frame : Bitfield<14, 1> {};
			struct End_of_frame : Bitfield<15, 1> {};
		};

		enum { MAX_BUFFER_COUNT = 1024 };

		DMA_POOL _dma_pool;

		void _reset_descriptor(unsigned const i, addr_t phys_addr) {
			if (i > _max_index())
				return;

			/* clear status */
			_descriptors[i].status = 0;

			/* set physical buffer address and set not used by SW
			 * last descriptor must be marked by Wrap bit
			 */
			_descriptors[i].addr =
				(phys_addr & Addr::Addr31to2::reg_mask())
				| Addr::Wrap::bits(i == _max_index());
		}

		inline bool _head_available()
		{
			return Addr::Used::get(_head().addr)
			    && Status::Length::get(_head().status);
		}

	public:
		static const size_t PACKET_SIZE = Nic::Packet_allocator::OFFSET_PACKET_SIZE;

		Rx_buffer_descriptor(Genode::Env          &,
		                     Platform::Connection &platform,
		                     SOURCE               &source)
		: Buffer_descriptor(platform, MAX_BUFFER_COUNT),
		  _dma_pool(platform, source)
		{
			for (size_t i=0; i <= _max_index(); i++) {
				try {
					Nic::Packet_descriptor p = source.alloc_packet(PACKET_SIZE);
					_reset_descriptor(i, _dma_pool.dma_addr(p));
				} catch (Nic::Session::Rx::Source::Packet_alloc_failed) {
					/* set new _buffer_count */
					_max_index(i-1);
					/* set wrap bit */
					_descriptors[_max_index()].addr |= Addr::Wrap::bits(1);
					break;
				}
			}

			Genode::log("Initialised ", _max_index()+1, " RX buffer descriptors");
		}

		bool reset_descriptor(Packet_descriptor pd)
		{
			addr_t const dma_addr = _dma_pool.dma_addr(pd);

			for (size_t i=0; i <= _max_index(); i++) {
				_advance_tail();
				if (Addr::Addr31to2::masked(_tail().addr) == dma_addr) {
					_reset_descriptor(_tail_index(), dma_addr);
					return true;
				}
			}
			return false;
		}

		void reset()
		{
			for (size_t i=0; i <= _max_index(); i++) {
				_descriptors[i].status = 0;
				Addr::Used::set(_descriptors[i].addr, 0);
			}
			_reset_head();
		}

		bool next_packet()
		{
			if (_head_available())
				return true;

			_advance_head();
			return _head_available();
		}

		Nic::Packet_descriptor get_packet_descriptor()
		{
			if (!_head_available())
				return Nic::Packet_descriptor(0, 0);

			const typename Status::access_t status = _head().status;
			if (!Status::Start_of_frame::get(status) || !Status::End_of_frame::get(status)) {
				warning("Packet split over more than one descriptor. Packet ignored!");

				_reset_descriptor(_head_index(), _head().addr);
				return Nic::Packet_descriptor(0, 0);
			}

			const size_t length = Status::Length::get(status);
			
			/* reset status */
			_head().status = 0;

			return _dma_pool.packet_descriptor_with_content((addr_t)Addr::Addr31to2::masked(_head().addr),
			                                                length);
		}

};

#endif /* _INCLUDE__DRIVERS__NIC__CADENCE_GEM__RX_BUFFER_DESCRIPTOR_H_ */
