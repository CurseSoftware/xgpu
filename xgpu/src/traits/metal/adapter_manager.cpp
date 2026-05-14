#include "utils/device.h"
#include "utils/utils.h"
#include "xgpu/api/api.h"
#include "xgpu/traits/metal/metal.h"

#include <iostream>

namespace xgpu::traits
{
    Adapter<>
    AdapterManager<core::GraphicsApi::Metal>::create_adapter(
        std::optional<data::PhysicalDevice> physical_device) noexcept
    {
        const std::span<data::PhysicalDevice> available_devices = enumerate_devices();
        if ( physical_device.has_value() ) {
            for ( const data::PhysicalDevice &device : available_devices ) {
                if ( device.name == physical_device->name ) {
                    return Adapter<>{ device };
                }
            }
        }

        return Adapter<>{ default_physical_device() };
    }
} // namespace xgpu::traits