#include "xgpu/components/components.h"
#include "xgpu/native/native.h"
#include "utils/device.h"
#include <string>

namespace xgpu::components
{
    std::span<data::PhysicalDevice>
    PhysicalDeviceManager<GraphicsApi::Metal>::enumerate_devices() noexcept
    {
        if (m_physical_devices.empty()) {
            m_physical_devices = retrieve_physical_devices();
        }

        return m_physical_devices;
    }

    data::PhysicalDevice
    PhysicalDeviceManager<GraphicsApi::Metal>::default_physical_device() noexcept
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
    PhysicalDeviceManager<GraphicsApi::Metal>::retrieve_physical_devices() noexcept
    {
        return mtl::get_available_physical_devices();
    }
} // namespace xgpu::components