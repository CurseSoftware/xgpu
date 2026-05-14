#include "utils/device.h"
#include "utils/utils.h"
#include "xgpu/api/api.h"
#include "xgpu/traits/metal/metal.h"
#include "xgpu/traits/traits.h"

#include <iostream>

namespace xgpu::traits
{
    Adapter<>
    AdapterManager<GraphicsApi::Metal>::create_adapter(
        std::optional<data::PhysicalDevice> physical_device) noexcept
    {
        const std::span<data::PhysicalDevice> available_devices = enumerate_devices();

        if ( physical_device.has_value() ) {
            for ( type_traits<GraphicsApi::Metal>::device_t device : mtl::all_devices() ) {
                if ( [[*device name] UTF8String] == physical_device->name ) {
                    return Adapter<>{
                        detail::AdapterContext<GraphicsApi::Metal>{ *physical_device, device }
                    };
                }
            }
        }

        return Adapter<>{
            detail::AdapterContext<GraphicsApi::Metal>{ default_physical_device(), mtl::default_device() }
        };
    }
} // namespace xgpu::traits