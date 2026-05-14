#pragma once
#include "metal_headers.h"
#include "xgpu/core/core.h"
#include "xgpu/traits/adapter_context.h"

namespace xgpu::traits::detail
{
    template <>
    class AdapterContext<core::GraphicsApi::Metal>
    {
      public:
        [[nodiscard]] constexpr AdapterContext(const data::PhysicalDevice physical_device) noexcept {}

        void
        create()
        {
        }

      private:
        OBJC_TYPE(id<MTLDevice>) m_device = nullptr;
        data::PhysicalDevice m_physical_device;
    };
} // namespace xgpu::traits::detail
