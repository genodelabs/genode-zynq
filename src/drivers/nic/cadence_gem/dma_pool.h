/*
 * \brief  Policy classes for obtaining DMA-capable memory buffers for packets.
 * \author Johannes Schlatow
 * \date   2022-02-08
 *
 * The DMA memory exactly mirrors the packet-buffer dataspace so that we can
 * reuse the packet-buffer management and thus simply calculate the DMA address
 * from a packet descriptor and vice versa.
 *
 * Note on alignment:
 * According to ug585, an alignment to cache line boundaries is beneficial
 * for performance but not mandatory. The packets from the packet allocator
 * actually offsets the packet address by 2-bytes. Since the allocated
 * buffer is actually cache-line aligned and the first two bytes of
 * the allocated buffer remain unused, I assume there is no
 * performance penalty.
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__NIC__CADENCE_GEM__DMA_POOL_H_
#define _DRIVERS__NIC__CADENCE_GEM__DMA_POOL_H_

/* Genode includes */
#include <platform_session/connection.h>
#include <os/packet_stream.h>
#include <cpu/cache.h>

namespace Cadence_gem
{
	using namespace Genode;
	using Packet_descriptor = Genode::Packet_descriptor;

	class Dma_pool_base;

	template <typename PACKET_STREAM>
	class Zerocopy_dma_pool;
}


class Cadence_gem::Dma_pool_base
{
	protected:
		addr_t const _dma_base_addr;
		size_t const _size;

	public:
		Dma_pool_base(addr_t dma_base, size_t size)
		: _dma_base_addr(dma_base),
		  _size(size)
		{ }

		/* return dma address for given packet descriptor */
		addr_t dma_addr(Packet_descriptor const &p) { return _dma_base_addr + p.offset(); }

		/* return dma address containing packet content of given packet descriptor */
		addr_t dma_addr_with_content(Packet_descriptor const &p);

		/* return packet descriptor for given dma address */
		Packet_descriptor packet_descriptor(addr_t dma_addr, size_t len)
		{
			if (dma_addr < _dma_base_addr || dma_addr + len > _dma_base_addr + _size)
				return Packet_descriptor(0, 0);

			return Packet_descriptor(dma_addr - _dma_base_addr, len);
		}

		/* return packet descriptor containing content from given dma address */
		Packet_descriptor packet_descriptor_with_content(addr_t dma_addr, size_t len);

};


template <typename PACKET_STREAM>
class Cadence_gem::Zerocopy_dma_pool : public Dma_pool_base
{
	private:
		PACKET_STREAM &_packet_stream;

	public:
		Packet_descriptor packet_descriptor_with_content(addr_t dma_addr, size_t len) {
			return packet_descriptor(dma_addr, len); }

		addr_t dma_addr_with_content(Packet_descriptor const &p)
		{
			cache_clean_invalidate_data(_packet_stream.ds_local_base() + p.offset(), p.size());
			return dma_addr(p);
		}

		Zerocopy_dma_pool(Platform::Connection &, PACKET_STREAM &ps)
		: Dma_pool_base(Dataspace_client(ps.dataspace()).phys_addr(), ps.ds_size()),
		  _packet_stream(ps)
		{
			if (!_dma_base_addr)
				error(__PRETTY_FUNCTION__, ": Could not get DMA address of dataspace");
		}
};

#endif /* _DRIVERS__NIC__CADENCE_GEM__DMA_POOL_H_ */

