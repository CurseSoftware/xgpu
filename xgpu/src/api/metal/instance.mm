#include "xgpu/api/api.h"
#include "utils/device.h"
#include <span>

namespace xgpu
{
    template <>
    Instance<GraphicsApi::Metal>::Instance(const InstanceDesc desc) {}

    template <>
    std::span<data::PhysicalDevice> Instance<GraphicsApi::Metal>::enumerate_devices() noexcept
    {
        if ( m_physical_devices.empty() )
        {
            m_physical_devices = metal::get_available_physical_devices();
        }

        return m_physical_devices;
    }

    template <>
    data::PhysicalDevice Instance<GraphicsApi::Metal>::default_physical_device() const noexcept
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
} // namespace xgpu