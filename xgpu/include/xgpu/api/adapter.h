#pragma once
#include "xgpu/core/core.h"
#include "xgpu/traits/metal/adapter_context.h"
#include "xgpu/traits/traits.h"

namespace xgpu
{
    template <
        GraphicsApi                           GAPI           = DefaultGraphicsApi,
        traits::detail::IAdapterContext<GAPI> AdapterContext = traits::detail::AdapterContext<GAPI>,
        traits::IQueueManager<GAPI>           QueuePolicy    = traits::QueueManager<GAPI>>
    class Adapter : public AdapterContext, public QueuePolicy
    {
    };
} // namespace xgpu