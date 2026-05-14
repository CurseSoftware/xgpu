#pragma once

#ifdef XGPU_COMPILE_METAL

#include "xgpu/core/core.h"
#include "xgpu/traits/physical_device_manager.h"

#include <span>
#include <vector>

/// @brief Metal implementation for the PhysicalDeviceManager trait
template <>
class xgpu::traits::PhysicalDeviceManager<xgpu::GraphicsApi::Metal>
{
  public:
    /// @brief Enumerate the available physical devices
    std::span<data::PhysicalDevice> enumerate_devices() noexcept;

    /// @brief Retrieve the default physical device
    data::PhysicalDevice            default_physical_device() noexcept;

  private:
    std::vector<data::PhysicalDevice> retrieve_physical_devices() noexcept;

    std::vector<data::PhysicalDevice> m_physical_devices;
};

#endif // XGPU_COMPILE_METAL