#include "device.h"
#include "xgpu/data/data.h"
#include "xgpu/traits/metal/metal.h"
#include <vector>

namespace xgpu::mtl
{
    std::vector<data::PhysicalDevice> get_available_devices() noexcept
    {
        std::vector<data::PhysicalDevice> physical_devices;
        NSArray<id<MTLDevice>> *devices = MTLCopyAllDevices();

        for (id<MTLDevice> device in devices) {
            data::PhysicalDevice physical_device;

            physical_device.name = [device.name UTF8String];
            physical_device.type = device.isLowPower
                                       ? data::PhysicalDeviceType::IntegratedGPU
                                       : data::PhysicalDeviceType::DiscreteGPU;
            physical_device.video_ram_bytes = device.recommendedMaxWorkingSetSize ?: 0;

            physical_devices.push_back(physical_device);
        }

        return physical_devices;
    }
} // namespace xgpu::mtl