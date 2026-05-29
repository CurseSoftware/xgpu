#include "xgpu/api/api.h"
#include "xgpu/core/core.h"
#include "utils/device.h"

#include <span>

#ifdef XGPU_COMPILE_METAL

namespace xgpu
{
    template <>
    expected<metal_instance, Error> metal_instance::create(const InstanceDesc &desc) noexcept
    {
        return metal_instance();
    }

    template <>
    std::span<data::PhysicalDevice> metal_instance::enumerate_devices() noexcept
    {
        if ( m_physical_devices.empty() )
        {
            m_physical_devices = metal::get_available_physical_devices();
        }

        return m_physical_devices;
    }

    template <>
    data::PhysicalDevice metal_instance::default_physical_device() const noexcept
    {
        type_traits<GraphicsApi::Metal>::device_t device = metal::default_device();

        return data::PhysicalDevice{
            .name = [device.name UTF8String],
            .type = device.isLowPower
                    ? data::PhysicalDeviceType::IntegratedGPU
                    : data::PhysicalDeviceType::DiscreteGPU,
            .video_ram_bytes = static_cast<std::uint32_t>(device.recommendedMaxWorkingSetSize ?: 0),
        };
    }

    template <>
    expected<Adapter<GraphicsApi::Metal>, Error> metal_instance::create_adapter(std::optional<data::PhysicalDevice> physical_device) noexcept
    {
        if ( physical_device )
        {
            for ( type_traits<GraphicsApi::Metal>::device_t device : metal::all_devices() )
            {
                if ( [[device name] UTF8String] == physical_device->name )
                {
                    return Adapter<>{
                        device,
                        physical_device.value()
                    };
                }
            }

            return unexpected<Error>(Error::make_error(ErrorCode::DeviceNotFound, physical_device->name));
        }

        return Adapter<>{
            metal::default_device(),
            default_physical_device()
        };
    }

    template <>
    void Instance<GraphicsApi::Metal>::destroy() noexcept
    {
        // metal instance destruction is a no-op
    }
} // namespace xgpu

#endif // XGPU_COMPILE_METAL