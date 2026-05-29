#pragma once
#include "instance_desc.h"
#include "primitive.h"
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"
#include "xgpu/types/types.h"

#include <span>

namespace xgpu
{
    template <GraphicsApi GAPI>
    class Adapter;

    /// @brief The entry point into an xgpu application
    template <GraphicsApi GAPI = DefaultGraphicsApi>
    class Instance : PrimitiveBase<Instance<GAPI>>
    {
      public:
        /// @brief Create an instance
        static expected<Instance, Error>              create(const InstanceDesc &desc) noexcept;

        /// @brief Retrieve the available devices to create an adapter from
        [[nodiscard]] std::span<data::PhysicalDevice> enumerate_devices() noexcept;

        /// @brief Get the default device
        [[nodiscard]] data::PhysicalDevice            default_physical_device() const noexcept;

        /// @brief Create an adapter
        /// @param physical_device If one is specified, choose a physical device to create the adapter from
        [[nodiscard]] expected<Adapter<GAPI>, Error>
             create_adapter(std::optional<data::PhysicalDevice> physical_device = std::nullopt) noexcept;

        /// @brief Destruction behavior for instance
        void destroy() noexcept;

      private:
        [[nodiscard]] explicit Instance() noexcept = default;

        /// @brief The internal instance for each backend
        typename type_traits<GAPI>::instance_t m_instance;
        std::vector<data::PhysicalDevice>      m_physical_devices;
    };

    /// @brief Create an instance
    template <GraphicsApi GAPI = DefaultGraphicsApi>
    expected<Instance<GAPI>, Error>
    create_instance(const InstanceDesc &desc) noexcept
    {
        return Instance<GAPI>::create(desc);
    }

    using metal_instance  = Instance<GraphicsApi::Metal>;
    using vulkan_instance = Instance<GraphicsApi::Vulkan>;
    using dx12_instance   = Instance<GraphicsApi::Dx12>;
} // namespace xgpu