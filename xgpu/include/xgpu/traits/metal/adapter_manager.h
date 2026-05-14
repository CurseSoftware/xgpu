#pragma once
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"
#include "xgpu/traits/adapter_manager.h"
#include "xgpu/traits/metal/physical_device_manager.h"

#include <optional>

namespace xgpu::traits
{
    template <>
    class AdapterManager<core::GraphicsApi::Metal> : public PhysicalDeviceManager<core::GraphicsApi::Metal>
    {
      public:
        /// @brief Create an adapter from a specified PhysicalDevice
        Adapter<core::GraphicsApi::Metal>
        create_adapter(std::optional<data::PhysicalDevice> physical_device = std::nullopt) noexcept;

      private:
    };
} // namespace xgpu::traits