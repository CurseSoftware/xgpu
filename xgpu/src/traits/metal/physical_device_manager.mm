#include "xgpu/traits/traits.h"
#include "xgpu/traits/metal/metal_headers.h"
#include "utils/device.h"
#include <string>

namespace xgpu::traits
{
    std::span<data::PhysicalDevice>
    PhysicalDeviceManager<core::GraphicsApi::Metal>::enumerate_devices() noexcept
    {
        if (m_physical_devices.empty()) {
            m_physical_devices = retrieve_physical_devices();
        }

        return m_physical_devices;
    }

    data::PhysicalDevice
    PhysicalDeviceManager<core::GraphicsApi::Metal>::default_physical_device() noexcept
    {
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();

        return data::PhysicalDevice {
            .name = [device.name UTF8String],
            .type = device.isLowPower
                    ? data::PhysicalDeviceType::IntegratedGPU
                    : data::PhysicalDeviceType::DiscreteGPU,
            .video_ram_bytes = static_cast<std::uint32_t>(device.recommendedMaxWorkingSetSize ?: 0),
        };
    }

    std::vector<data::PhysicalDevice>
    PhysicalDeviceManager<core::GraphicsApi::Metal>::retrieve_physical_devices() noexcept
    {
        return mtl::get_available_devices();
//        std::vector<data::PhysicalDevice> physical_devices;
//        NSArray<id<MTLDevice>> *devices = MTLCopyAllDevices();
//
//        for (id<MTLDevice> device in devices) {
//            data::PhysicalDevice physical_device;
//
//            physical_device.name = [device.name UTF8String];
//            physical_device.type = device.isLowPower
//                                       ? data::PhysicalDeviceType::IntegratedGPU
//                                       : data::PhysicalDeviceType::DiscreteGPU;
//            physical_device.video_ram_bytes = device.recommendedMaxWorkingSetSize ?: 0;
//
//            physical_devices.push_back(physical_device);
//        }
//
//        return physical_devices;
    }
} // namespace xgpu::traits