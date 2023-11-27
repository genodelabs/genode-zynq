/*
 * \brief  Zynq DMA guard implementation
 * \author Johannes Schlatow
 * \date   2022-12-20
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__DRIVERS__PLATFORM__DMA_GUARD_H_
#define _SRC__DRIVERS__PLATFORM__DMA_GUARD_H_

/* Genode includes */
#include <os/attached_mmio.h>
#include <util/register_set.h>
#include <util/misc_math.h>
#include <base/allocator.h>

/* Platform-driver includes */
#include <common.h>
#include <device.h>
#include <io_mmu.h>
#include <dma_allocator.h>

namespace Driver {
	using namespace Genode;

	class Dma_guard;
	class Dma_guard_factory;
}


class Driver::Dma_guard : private Attached_mmio,
                          public  Driver::Io_mmu
{
	public:

		/* Use derived domain class to store reference to buffer registry */
		class Domain : public Driver::Io_mmu::Domain
		{
			private:

				Dma_guard                  & _dma_guard;
				Registry<Dma_buffer> const & _buffer_registry;

			public:

				void enable_pci_device(Io_mem_dataspace_capability const, Pci::Bdf const &) override { };
				void disable_pci_device(Pci::Bdf const &) override { };

				void add_range(Range const & range, addr_t,
				               Dataspace_capability const) override {
					_dma_guard._add_range(range); }

				void remove_range(Range const & range) override {
					_dma_guard._remove_range(range); }

				Domain(Dma_guard & dma_guard, Allocator & md_alloc, Registry<Dma_buffer> const & buffer_registry)
				: Driver::Io_mmu::Domain(dma_guard, md_alloc),
				  _dma_guard(dma_guard),
				  _buffer_registry(buffer_registry)
				{
					_buffer_registry.for_each([&] (Dma_buffer const & buf) {
						add_range({ buf.dma_addr, buf.size }, buf.phys_addr, buf.cap); });
				}

				~Domain() override
				{
					_buffer_registry.for_each([&] (Dma_buffer const & buf) {
						remove_range({ buf.dma_addr, buf.size }); });
				}
		};

	private:

		enum {
			NUM_SEGMENTS = 10
		};

		struct Ctrl : Register<0x0, 32>
		{
			struct Enable : Bitfield<0,2>
			{
				enum {
					READ_WRITE = 0x0,
					WRITE_ONLY = 0x1,
					READ_ONLY  = 0x2,
					DENY       = 0x3
				};
			};
		};

		struct Segments : Register_array<0x4, 32, NUM_SEGMENTS, 32>
		{
			struct Valid     : Bitfield< 0, 1> { };
			struct Writeable : Bitfield< 1, 1> { };
			struct Size      : Bitfield< 4, 8> { };
			struct Addr      : Bitfield<12,20> { };
		};

		void _add_range(Range);
		void _remove_range(Range);

		/**
		 * Iommu interface
		 */

		void _enable() override {
			write<Ctrl::Enable>(Ctrl::Enable::READ_WRITE); }

		void _disable() override {
			write<Ctrl::Enable>(Ctrl::Enable::DENY); }

	public:

		/**
		 * Iommu interface
		 */

		bool mpu() const override { return true; }

		Driver::Io_mmu::Domain & create_domain(Allocator & md_alloc,
		                                       Ram_allocator &,
		                                       Registry<Dma_buffer> const & buffer_registry,
		                                       Ram_quota_guard &,
		                                       Cap_quota_guard &) override
		{
			return *new (md_alloc) Dma_guard::Domain(*this, md_alloc, buffer_registry);
		}


		Dma_guard(Env                      & env,
		          Io_mmu_devices           & io_mmu_devices,
		          Device::Name       const & name,
		          Device::Io_mem::Range      range)
		: Attached_mmio(env, range.start, range.size),
		  Io_mmu(io_mmu_devices, name)
		{ };

		~Dma_guard() { _destroy_domains(); }
};


class Driver::Dma_guard_factory : public Driver::Io_mmu_factory
{
	private:

		Genode::Env  & _env;

	public:

		Dma_guard_factory(Genode::Env & env, Common & common)
		: Io_mmu_factory(common.io_mmu_factories(), Device::Type { "dma_guard" }),
		  _env(env)
		{ }

		void create(Allocator & alloc, Io_mmu_devices & io_mmu_devices, Device const & device) override
		{
			using Range = Device::Io_mem::Range;

			device.for_each_io_mem([&] (unsigned idx, Range range, Device::Pci_bar, bool)
			{
				if (idx == 0)
					new (alloc) Dma_guard(_env, io_mmu_devices, device.name(), range);
			});
		}
};


void Driver::Dma_guard::_add_range(Range range)
{
	bool found = false;

	/* calculate log2 size */
	size_t size_log2 = log2(range.size);
	if ((1U << size_log2) < range.size) size_log2++;

	for (size_t i=0; !found && i < NUM_SEGMENTS; i++) {
		if (!read<Segments::Valid>(i)) {
			write<Segments>(
				Segments::Valid::bits(1) |
				Segments::Writeable::bits(1) |
				Segments::Addr::masked(range.start) |
				Segments::Size::bits(size_log2-2),
				i);
			found = true;
		}
	}

	if (!found)
		error(__func__, "() all segment registers are already in use");
}


void Driver::Dma_guard::_remove_range(Range range)
{
	bool found = false;
	uint32_t addr_field = Segments::Addr::get(range.start);

	/* find segment for this address and invalidate */
	size_t i=0;
	for (; !found && i < NUM_SEGMENTS; i++) {
		if (read<Segments::Valid>(i) && read<Segments::Addr>(i) == addr_field) {
			write<Segments::Valid>(0, i);
			found = true;
		}
	}

	if (!found)
		warning(__func__, "() unable to find segment");

	/* move remaining segment registers to fill the space of the invalidated segment */
	for (; i < NUM_SEGMENTS; i++) {
		if (!read<Segments::Valid>(i))
			break;

		write<Segments>(read<Segments>(i), i-1);
	}
}

#endif /* _SRC__DRIVERS__PLATFORM__DMA_GUARD_H_ */
