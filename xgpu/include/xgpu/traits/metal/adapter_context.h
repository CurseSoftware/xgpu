#pragma once
#include "../../native/metal_headers.h"
#include "xgpu/core/core.h"
#include "xgpu/traits/adapter_context.h"

namespace xgpu::traits::detail
{
    template <>
    class AdapterContext<GraphicsApi::Metal>
    {
      public:
        [[nodiscard]] constexpr explicit(false) AdapterContext(
            const data::PhysicalDevice               &physical_device,
            type_traits<GraphicsApi::Metal>::device_t device) noexcept
            : m_device(device), m_physical_device(physical_device)
        {
        }

        /// @brief Get the physical device for this adapter
        [[nodiscard]] constexpr const data::PhysicalDevice &
        physical_device() const noexcept
        {
            return m_physical_device;
        }

        /// @brief Get the underlying metal device
        [[nodiscard]] constexpr type_traits<GraphicsApi::Metal>::device_t
        device() const noexcept
        {
            return m_device;
        }

      private:
        type_traits<GraphicsApi::Metal>::device_t m_device = nullptr;
        data::PhysicalDevice                      m_physical_device;
    };
} // namespace xgpu::traits::detail
