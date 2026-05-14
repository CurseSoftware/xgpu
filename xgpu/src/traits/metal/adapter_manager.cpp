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
        if ( physical_device.has_value() ) {
            for ( const std::vector<data::PhysicalDevice> available_devices = mtl::get_available_devices();
                  const data::PhysicalDevice             &device : available_devices ) {
                if ( device.name == physical_device->name ) {
                    std::cout << "FOUND DEVICE" << std::endl;
                }
            }
        }

        return {};
    }
} // namespace xgpu::traits