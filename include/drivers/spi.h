/*
 * \brief  Zynq SPI driver
 * \author Johannes Schlatow
 * \date   2021-10-20
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__DRIVERS__SPI_H_
#define _INCLUDE__DRIVERS__SPI_H_

#include <platform_session/connection.h>
#include <platform_session/device.h>

namespace Spi {
	using namespace Genode;

	class Mmio;
	class Zynq_driver;
}

struct Spi::Mmio : Platform::Device::Mmio
{
	struct Ctrl : Register<0x00, 32>
	{
		struct Master       : Bitfield< 0,1> { };
		struct Cpol         : Bitfield< 1,1> { };
		struct Cpha         : Bitfield< 2,1> { };
		struct Prescaler    : Bitfield< 3,3>
		{
			enum {
				INVALID = 0b000,
				MIN     = 0b001, /*   4 */
				DEFAULT = 0b101, /*  64 */
				MAX     = 0b111, /* 256 */
			};
		};
		struct Manual_start : Bitfield<15,1> { };
		struct Transfer     : Bitfield<16,1> { };
	};

	struct Status : Register<0x04, 32>
	{
		struct Tx_not_full : Bitfield<2,1> { };
	};

	struct Enable : Register<0x14, 32> { };
	
	struct Tx : Register<0x1c, 32>
	{
		struct Data : Bitfield<0,8> { };
	};

	struct Rx : Register<0x20, 32>
	{
		struct Data : Bitfield<0,8> { };
	};

	struct Module : Register<0xfc, 32> { };

	Mmio(Platform::Device &device)
	: Platform::Device::Mmio(device)
	{ }
};

class Spi::Zynq_driver
{
	public:
		typedef Platform::Device::Type Type;

	private:

		Platform::Device       _device;
		Platform::Device::Mmio _mmio;

	public:

		Zynq_driver(Platform::Connection &platform, Type const &type)
		: _device(platform, type),
		  _mmio(_device)
		{
			/**
			 * By default, the controller preforms chip select and starts transfers
			 * automatically.
			 * Hence, we set master mode, polarity, phase, clock divisor and
			 * manual start mode.
			 */

			using String = String<64>;

			/* default property values */
			unsigned max_freq   = 0;
			unsigned clock_freq = 0;
			bool     cpha = false;
			bool     cpol = false;

			/* read properties from platform session */
			bool found = false;
			platform.with_xml([&] (Xml_node & xml) {
				xml.for_each_sub_node("device", [&] (Xml_node & node) {
					if (found)
						return;

					if (node.attribute_value("type", String()) != type.name)
						return;

					node.for_each_sub_node("property", [&] (Xml_node &prop) {
						const String name = prop.attribute_value("name", String());
						if (name == "spi-max-frequency")
							max_freq = prop.attribute_value("value", max_freq);

						if (name == "input-clock")
							clock_freq = prop.attribute_value("value", clock_freq);

						else if (name == "spi-cpha")
							cpha = prop.attribute_value("value", cpha);

						else if (name == "spi-cpol")
							cpol = prop.attribute_value("value", cpol);
					});

					found = true;
				});
			});

			if (clock_freq == 0)
				error("Unable to set spi clock because input-clock <property> is missing.");
			if (max_freq == 0)
				error("Unable to set spi clock because spi-max-frequency <property> is missing.");

			/* always set master mode */
			_mmio.write<Mmio::Ctrl::Master>(1);

			/* calculate clock prescaler */
			unsigned prescaler  = 0;
			if (clock_freq != 0 && max_freq != 0) {
				unsigned div        = clock_freq / max_freq;
				bool     power_of_2 = !(div & (div-1)) && !(clock_freq % max_freq);

				while (div) {
					prescaler += 1;
					div >>= 1;
				}

				if (power_of_2)
					prescaler -= 2;
				else
					prescaler -= 1;
			}

			/* use default setting if out of bounds */
			if (prescaler < Mmio::Ctrl::Prescaler::MIN || prescaler > Mmio::Ctrl::Prescaler::MAX)
				prescaler = Mmio::Ctrl::Prescaler::DEFAULT;

			_mmio.write<Mmio::Ctrl::Prescaler>(prescaler);

			/* set Cpol and Cpha */
			_mmio.write<Mmio::Ctrl::Cpol>(cpol);
			_mmio.write<Mmio::Ctrl::Cpha>(cpha);
		}

		unsigned id() { return _mmio.read<Mmio::Module>(); }

		size_t write_and_read(unsigned char *buf, size_t bytes_to_send)
		{
			size_t bytes_sent     = 0;
			size_t bytes_received = 0;

			/**
			 * Since we only transfer small amounts of data, we don't use
			 * interrupt handling but fill the Tx FIFO only once. This limits
			 * the maximum buffer length to 128 bytes, though.
			 */
			if (bytes_to_send > 128) {
				error("Buffer exceeds FIFO length, only sending first 128 bytes.");
				bytes_to_send = 128;
			}

			/* enable controller */
			_mmio.write<Mmio::Enable>(1);

			/* TODO (optional) set chip select */

			/* fill Tx FIFO */
			while (bytes_sent < bytes_to_send)
				_mmio.write<Mmio::Tx::Data>(buf[bytes_sent++]);

			/* start the transfer */
			_mmio.write<Mmio::Ctrl::Transfer>(1);

			/* wait until Tx FIFO is empty */
			while (!_mmio.read<Mmio::Status::Tx_not_full>());

			/* read from Rx FIFO */
			while (bytes_received < bytes_sent)
				buf[bytes_received++] = _mmio.read<Mmio::Rx::Data>();

			/* disable controller */
			_mmio.write<Mmio::Enable>(0);

			return bytes_received;
		}
};



#endif /* _INCLUDE__DRIVERS__SPI_H_ */
