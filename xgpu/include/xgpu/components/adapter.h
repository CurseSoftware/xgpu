#pragma once
#include "detail/adapter_context.h"
#include "xgpu/components/component.h"
#include "xgpu/core/core.h"

namespace xgpu::components
{
    /// @brief Adapter context
    /// @note This should be remain undefined and only be defined through specializations
    // template <GraphicsApi GAPI>
    // struct AdapterContext;

    template <GraphicsApi GAPI>
    using AdapterComponent = Component<detail::AdapterContext<GAPI>>;
} // namespace xgpu::components