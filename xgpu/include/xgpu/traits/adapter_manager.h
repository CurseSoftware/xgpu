#pragma once
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"

namespace xgpu
{
    template <core::GraphicsApi GAPI>
    class Adapter;
} // namespace xgpu

namespace xgpu::traits
{
    /// @brief Requirements for a valid adapter manager type
    template <class T, core::GraphicsApi GAPI>
    concept IAdapterManager = requires(T adapter_manager) {
        { adapter_manager.create_adapter() } -> std::same_as<class Adapter<GAPI>>;
    };

    /// @brief Named interface class that should never be instantiated
    template <core::GraphicsApi GAPI>
    class AdapterManager;
} // namespace xgpu::traits