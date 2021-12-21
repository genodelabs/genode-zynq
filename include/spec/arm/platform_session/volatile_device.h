/*
 * \brief  Covenience interface for managing volatile platform devices
 * \author Johannes Schlatow
 * \date   2021-12-20
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__SPEC__ARM__PLATFORM_SESSION__VOLATILE_DEVICE_H_
#define _INCLUDE__SPEC__ARM__PLATFORM_SESSION__VOLATILE_DEVICE_H_

#include <util/reconstructible.h>

namespace Platform {
	class Volatile_device;

	template <typename T, typename ID>
	class Volatile_driver;
}


class Platform::Volatile_device
{
	public:
		struct Device_unavailable : Genode::Exception { };

	private:
		Genode::Constructible<Platform::Device>   _device { };

	public:
		bool available() {
			return _device.constructed(); }

		Platform::Device &device()
		{
			if (!available())
				throw Device_unavailable();

			return *_device;
		}

		template <typename... ARGS>
		void acquire(ARGS &&... args)
		{
			if (_device.constructed())
				_device.destruct();

			_device.construct(args...);
		}
};


/**
 * Convenience wrapper for dynamically constructing a driver object of the
 * given type, when the corresponding device becomes available.
 *
 * The template parameter T specifies the driver type. The constructor must take
 * a Platform::Device& as first argument.
 * Parameter ID specifies the identifier type (Device::Type or Device::Name)
 * of the platform device.
 */
template <typename T, typename ID>
class Platform::Volatile_driver
{
	public:
		typedef T Driver;

	protected:
		Platform::Connection     &_platform;
		ID                        _identifier;

	private:
		Volatile_device           _device      { };
		Genode::Constructible<T>  _driver      { };

	public:
		Volatile_driver(Platform::Connection &platform, ID identifier)
		: _platform(platform),
		  _identifier(identifier)
		{ }

		bool available()
		{
			if (!_device.available() && _driver.constructed())
				_driver.destruct();

			return _driver.constructed();
		}

		T &driver()
		{
			if (!available())
				throw Volatile_device::Device_unavailable();

			return *_driver;
		}

		template <typename... ARGS>
		void acquire(ARGS &&... args)
		{
			if (_driver.constructed())
				_driver.destruct();

			if (!_device.available())
				_device.acquire(_platform, _identifier);

			_driver.construct(_device.device(), args...);
		}
};


#endif /* _INCLUDE__SPEC__ARM__PLATFORM_SESSION__VOLATILE_DEVICE_H_ */
