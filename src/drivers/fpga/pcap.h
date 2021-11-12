/*
 * \brief  PCAP driver for FPGA configuration
 * \author Johannes Schlatow
 * \date   2021-11-12
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__DRIVERS__FPGA__PCAP_H_
#define _INCLUDE__DRIVERS__FPGA__PCAP_H_

#include <platform_session/device.h>
#include <os/reporter.h>

namespace Fpga {
	using namespace Genode;

	class Pcap_loader;
}

class Fpga::Pcap_loader : public Platform::Device::Mmio
{
	public:
		using Name = String<32>;

	private:
		Env                    &_env;
		Platform::Connection   &_platform;
		Reporter               &_reporter;

		struct Ctrl : Register<0x0, 32>
		{
			struct Rate : Bitfield<25,1>
			{
				enum { NORMAL  = 0,
				       QUARTER = 1 };
			};

			struct Mode : Bitfield<26,2>
			{
				enum { ENABLE = 0x3 };
			};

			struct Prog_b : Bitfield<30,1> { };
		};

		struct Interrupts : Register<0x0C, 32>
		{
			struct Pfg_done : Bitfield< 2,1> { };
			struct Dma_done : Bitfield<13,1> { };

			struct Seu_err       : Bitfield< 5,1> { };
			struct Hmac_err      : Bitfield< 6,1> { };
			struct P2d_len_err   : Bitfield<11,1> { };
			struct Dma_overflow  : Bitfield<14,1> { };
			struct Dma_err       : Bitfield<15,1> { };
			struct Fifo_overflow : Bitfield<18,1> { };
			struct Axi_rd_err    : Bitfield<20,1> { };
			struct Axi_wr_err    : Bitfield<22,1> { };

			struct Errors : Bitset_3<Bitset_3<Seu_err, Hmac_err, P2d_len_err>,
			                         Bitset_3<Dma_overflow, Dma_err, Fifo_overflow>,
			                         Bitset_2<Axi_rd_err, Axi_wr_err>> { };
		};

		struct Status : Register<0x14, 32>
		{
			struct Pfg_init : Bitfield< 4,1> { };
			struct Dma_full : Bitfield<31,1> { };
		};

		struct Dma_src : Register<0x18, 32>
		{
			enum { FPGA = ~0U };
			enum { WAIT_FOR_PCAP_DONE = 0x1 };
		};

		struct Dma_dst : Register<0x1C, 32>
		{
			enum { FPGA = ~0U };
		};

		struct Dma_src_len : Register<0x20, 32>
		{
			struct Words : Bitfield<0,27> { };
		};

		struct Dma_dst_len : Register<0x24, 32>
		{
			struct Words : Bitfield<0,27> { };
		};

		struct Mctrl : Register<0x80, 32>
		{
			struct Loopback : Bitfield< 4,1> { };
			struct Por      : Bitfield< 8,1> { };
			struct Version  : Bitfield<28,4> { };
		};


		struct Dma_buffer
		{
			Platform::Connection       &_platform;
			Ram_dataspace_capability    _ds_cap;
			Genode::Attached_dataspace  _ds;
			addr_t const                _dma_addr { _platform.dma_addr(_ds_cap) };

			Dma_buffer(Genode::Env & env, Platform::Connection & platform, size_t size)
			: _platform(platform),
			  _ds_cap(_platform.alloc_dma_buffer(size, UNCACHED)),
			  _ds(env.rm(), _ds_cap)
			{ }

			~Dma_buffer() { _platform.free_dma_buffer(_ds_cap); }

			char *local_addr() { return (char*)_ds.local_addr<addr_t>(); };
		};
		

		bool _device_valid() { return read<Mctrl::Version>() > 0; }

		void _report(Name const &name, bool loaded)
		{
			Reporter::Xml_generator xml(_reporter, [&] () {
				xml.node("bitstream", [&] () {
					if (name != "")
						xml.attribute("name", name);
					xml.attribute("loaded", loaded);
				});
			});
		}

	public:

		Pcap_loader(Env                    &env,
		            Platform::Connection   &platform,
		            Platform::Device       &device,
		            Reporter               &reporter)
		: Platform::Device::Mmio(device),
		  _env(env),
		  _platform(platform),
		  _reporter(reporter)
		{ }

		/**
		 * Reset PL by following the sequence from
		 * ug585-Zynq-7000-TRM v1.13 p.212+213.
		 */
		void reset()
		{
			if (!_device_valid()) {
				error("Invalid devcfg device");
				return;
			}

			/* report that we unload the bitstream */
			_report("", false);

			write<Ctrl::Prog_b>(1);
			write<Ctrl::Prog_b>(0);

			while (read<Status::Pfg_init>());

			write<Ctrl::Prog_b>(1);
			write<Interrupts::Pfg_done>(1);

			while (!read<Status::Pfg_init>());
		}
		

		template <typename TRANSFER>
		void load_bitstream(size_t size, Name const &name, TRANSFER && transfer)
		{
			if (!_device_valid()) {
				error("Invalid devcfg device");
				return;
			}

			/* allocate DMA buffer */
			Dma_buffer dma_buf { _env, _platform, size };

			/* copy bitstream into DMA buffer */
			size_t transferred = transfer(dma_buf.local_addr());
			if (transferred != size) {
				error("Failed copying ", size, " bytes to DMA buffer");
				return;
			}
			
			/* enable PCAP interface */
			write<Ctrl::Mode>(Ctrl::Mode::ENABLE);

			/* clear interrupts */
			write<Interrupts>(~0U);

			/* reset the PL */
			reset();

			/* disable PCAP loopback */
			write<Mctrl::Loopback>(0);

			/* set PCAP clock divider to the rate expected in non-secure mode */
			write<Ctrl::Rate>(Ctrl::Rate::NORMAL);

			/* set DMA source address */
			write<Dma_src>(dma_buf._dma_addr | Dma_src::WAIT_FOR_PCAP_DONE);

			/* set DMA destination address = FPGA */
			write<Dma_dst>(Dma_dst::FPGA);

			/* set DMA source length (32bit words */
			write<Dma_src_len::Words>(size >> 2);

			/* set DMA destination length - finalises DMA command */
			write<Dma_dst_len::Words>(size >> 2);

			/* wait for DMA done bit (includes PCAP done) */
			while (!read<Interrupts::Dma_done>());

			/* check for errors */
			bool failed = read<Interrupts::Errors>();

			/* make sure the PL is done */
			if (!failed)
				while (!read<Interrupts::Pfg_done>());

			/* report state */
			_report(name, !failed);
		}
};

#endif /* _INCLUDE__DRIVERS__FPGA__PCAP_H_ */
