#include "utils/device.h"
#include "utils/utils.h"
#include "xgpu/api/api.h"
#include "xgpu/components/metal/metal.h"
#include "xgpu/components/components.h"

#include <iostream>

namespace xgpu::components
{
    Adapter<>
    AdapterManager<GraphicsApi::Metal>::create_adapter(
        std::optional<data::PhysicalDevice> physical_device) noexcept
    {
        const std::span<data::PhysicalDevice> available_devices = enumerate_devices();

        if ( physical_device.has_value() ) {
            for ( type_traits<GraphicsApi::Metal>::device_t device : mtl::all_devices() ) {
                if ( [[device name] UTF8String] == physical_device->name ) {
                    return Adapter<>{
                        device,
                        physical_device.value()
                    };
                }
            }
        }

        return Adapter<>{
            mtl::default_device(),
            default_physical_device()
        };
    }
} // namespace xgpu::components