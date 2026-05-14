#pragma once
#include "xgpu/core/core.h"
#include "xgpu/traits/adapter_manager.h"

namespace xgpu::traits
{
    template <>
    class AdapterManager<core::GraphicsApi::Metal>
    {
      public:
        /// @brief Create an adapter from a specified PhysicalDevice
        Adapter<core::GraphicsApi::Metal> create_adapter() noexcept;

      private:
    };
} // namespace xgpu::traits