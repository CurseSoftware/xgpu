#pragma once
#include "xgpu/components/adapter_manager.h"
#include "xgpu/components/metal/physical_device_manager.h"
// #include "xgpu/components/metal/queue_manager.h"
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"

#include <optional>

namespace xgpu::components
{
    template <>
    class AdapterManager<GraphicsApi::Metal> : public PhysicalDeviceManager<GraphicsApi::Metal>
    {
      public:
        /// @brief Create an adapter from a specified PhysicalDevice
        Adapter<GraphicsApi::Metal>
        create_adapter(std::optional<data::PhysicalDevice> physical_device = std::nullopt) noexcept;

      private:
    };
} // namespace xgpu::traits