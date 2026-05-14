#pragma once
#include "xgpu/core/core.h"
#include "xgpu/traits/metal/adapter_context.h"
#include "xgpu/traits/traits.h"

namespace xgpu
{
    template <
        core::GraphicsApi                     GAPI    = core::DefaultGraphicsApi,
        traits::detail::IAdapterContext<GAPI> Context = traits::detail::AdapterContext<GAPI>>
    class Adapter : public Context
    {
    };
} // namespace xgpu