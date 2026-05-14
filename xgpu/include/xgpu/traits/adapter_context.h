#pragma once
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"
#include "xgpu/types/types.h"
#include <concepts>

namespace xgpu::traits::detail
{
    /// @brief The interface for a valid AdapterContext
    template <typename T, GraphicsApi GAPI = DefaultGraphicsApi>
    concept IAdapterContext
        = std::constructible_from<T, const data::PhysicalDevice &, typename type_traits<GAPI>::device_t>;

    /// @brief Internal context of the adapter used by consumer policies
    template <GraphicsApi GAPI>
    class AdapterContext;
} // namespace xgpu::traits::detail