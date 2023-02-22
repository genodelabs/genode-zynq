/*
 * \brief  Platform driver - control device
 j \author Johannes Schlatow
 * \date   2023-01-20
 */

/*
 * Copyright (C) 2023 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__DRIVERS__PLATFORM__CONTROL_DEVICE_H_
#define _SRC__DRIVERS__PLATFORM__CONTROL_DEVICE_H_

#include <base/registry.h>

#include <device.h>

namespace Driver
{
	using namespace Genode;

	class  Control_device;
	class  Control_device_factory;

	using Control_devices = Registry<Control_device>;
}


class Driver::Control_device : private Control_devices::Element
{
	public:

		using Range = Platform::Device_interface::Range;

		struct Domain_id
		{
			int id { -1 };

			bool valid() { return id >= 0; }
		};

		class Domain : private Registry<Domain>::Element
		{
			private:

				friend class Control_device;

				Control_device & _control_device;
				Allocator      & _md_alloc;
				Domain_id        _domain_id { _control_device._alloc_domain_id() };

				unsigned         _active_devices { 0 };

			public:

				Device::Name const & device_name() const { return _control_device.name(); }

				void enable_device()
				{
					_active_devices++;

					if (_active_devices == 1)
						_control_device._enable_domain();
				}

				void disable_device()
				{
					if (_active_devices > 0) {
						_active_devices--;

						if (_active_devices == 0)
							_control_device._disable_domain();
					}
				}

				unsigned devices() const { return _active_devices; }

				void add_range(Range range)    { _control_device._add_range(range); }
				void remove_range(Range range) { _control_device._remove_range(range); }

				Domain(Control_device & control_device, Allocator & md_alloc)
				: Registry<Domain>::Element(control_device._domains, *this),
				  _control_device(control_device), _md_alloc(md_alloc)
				{ }

				virtual ~Domain() { }
		};

	protected:

		friend class Domain;

		Device::Name      _name;
		Registry<Domain>  _domains { };

		unsigned          _active_domains { 0 };

		virtual void      _enable()  { };
		virtual void      _disable() { };
		virtual Domain_id _alloc_domain_id() { return Domain_id(); };

		/* Add a memory range (e.g. DMA buffer) */
		virtual void _add_range(Range) { };

		/* Remove a previously added memory range */
		virtual void _remove_range(Range) { };

		void _enable_domain()
		{
			if (!_active_domains)
				_enable();

			_active_domains++;
		};

		void _disable_domain()
		{
			if (_active_domains > 0)
				_active_domains--;

			if (!_active_domains)
				_disable();
		};

		void _destroy_domains()
		{
			_domains.for_each([&] (Domain & domain) {
				destroy(domain._md_alloc, &domain); });
		}

	public:

		Device::Name const & name() const { return _name; }

		bool domain_owner(Domain const & domain) const {
			return &domain._control_device == this; }

		Control_device(Control_devices     & devices,
		               Device::Name  const & name)
		: Control_devices::Element(devices, *this),
			_name(name)
		{ }

		virtual ~Control_device()
		{
			/**
			 * destroying domain objects
			 * any derived class that overrides any virtual method must
			 * call this at the very beginning of its
			 */
			_destroy_domains();
		}
};


class Driver::Control_device_factory : private Genode::Registry<Control_device_factory>::Element
{
	protected:

		Device::Type  _type;

	public:

		Control_device_factory(Registry<Control_device_factory> & registry,
		                       Device::Type               const & type)
		: Registry<Control_device_factory>::Element(registry, *this),
		  _type(type)
		{ }

		virtual ~Control_device_factory() { }

		bool matches(Device const & dev) {
			return dev.type() == _type; }

		virtual void create(Allocator &,
		                    Control_devices &,
		                    Device const &) = 0;
};


#endif /* _SRC__DRIVERS__PLATFORM__CONTROL_DEVICE_H_ */
