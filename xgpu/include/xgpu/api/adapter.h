#pragma once
#include "instance.h"
#include "primitive.h"
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"
#include "xgpu/types/types.h"

#include <utility>

namespace xgpu
{
    // Forward declaration for friend class
    template <GraphicsApi GAPI>
    class Instance;

    template <GraphicsApi GAPI = DefaultGraphicsApi>
    class Adapter : PrimitiveBase<Adapter<GAPI>>
    {
      public:
        /// @brief Get the physical device information for this adapter
        [[nodiscard]] data::PhysicalDevice
        physical_device() const noexcept
        {
            return m_physical_device;
        }

        /// @brief Destroy and release the adapter
        void destroy() noexcept;

      private:
        friend class Instance<GAPI>;
        [[nodiscard]] explicit Adapter(
            type_traits<GraphicsApi::Metal>::device_t device, data::PhysicalDevice physical_device) noexcept
            : m_device{ device }, m_physical_device{ std::move(physical_device) }
        {
        }

        type_traits<GraphicsApi::Metal>::device_t m_device;
        data::PhysicalDevice                      m_physical_device;
    };

    using metal_adapter  = Adapter<GraphicsApi::Metal>;
    using vulkan_adapter = Adapter<GraphicsApi::Vulkan>;
    using dx12_adapter   = Adapter<GraphicsApi::Dx12>;
} // namespace xgpu