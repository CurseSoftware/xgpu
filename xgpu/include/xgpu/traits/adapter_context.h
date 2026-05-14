#pragma once
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"
#include <concepts>

namespace xgpu::traits::detail
{
    template <typename T, core::GraphicsApi GAPI = core::DefaultGraphicsApi>
    concept IAdapterContext = std::constructible_from<const data::PhysicalDevice>;

    /// @brief Internal context of the adapter used by consumer policies
    template <core::GraphicsApi GAPI>
    class AdapterContext;
} // namespace xgpu::traits::detail