#include "xgpu/traits/traits.h"
#include <string>

namespace xgpu::traits
{
    std::span<data::PhysicalDevice>
    PhysicalDeviceManager<core::GraphicsApi::Metal>::enumerate_devices() noexcept
    {
        return m_physical_devices;
    }

    data::PhysicalDevice
    PhysicalDeviceManager<core::GraphicsApi::Metal>::default_physical_device() noexcept
    {
        return m_physical_devices[0];
    }
} // namespace xgpu::traits