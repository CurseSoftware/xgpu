#pragma once
#include "instance_desc.h"
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"
#include "xgpu/types/types.h"

#include <span>

namespace xgpu
{
    /// @brief The entry point into an xgpu application
    template <GraphicsApi GAPI = DefaultGraphicsApi>
    class Instance
    {
      public:
        [[nodiscard]] explicit Instance(const InstanceDesc desc) {}

        std::span<data::PhysicalDevice> enumerate_devices() noexcept;

        data::PhysicalDevice            default_physical_device() const noexcept;

      private:
        /// @brief The internal instance for each backend
        typename type_traits<GAPI>::instance_t m_instance;
        std::vector<data::PhysicalDevice>      m_physical_devices;
    };

    // extern template class Instance<GraphicsApi::Metal>;
} // namespace xgpu