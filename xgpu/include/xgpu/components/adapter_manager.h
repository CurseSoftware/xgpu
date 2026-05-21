#pragma once
#include "detail/adapter_context.h"
#include "physical_device_manager.h"
#include "queue_manager.h"
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"

#include <optional>

namespace xgpu
{
    template <GraphicsApi GAPI>
    class Adapter;
} // namespace xgpu

namespace xgpu::components
{
    /// @brief Requirements for a valid adapter manager type
    template <class T, GraphicsApi GAPI>
    concept IAdapterManager
        = requires(T adapter_manager, std::optional<data::PhysicalDevice> physical_device) {
              { adapter_manager.create_adapter(physical_device) } -> std::same_as<class Adapter<GAPI>>;
          } && concepts::ContainsMixin<T, PhysicalDeviceManager<GAPI>>;

    /// @brief Handles the creation of Adapters
    /// @note Named interface class that should never be instantiated
    template <GraphicsApi GAPI>
    class AdapterManager;
} // namespace xgpu::components