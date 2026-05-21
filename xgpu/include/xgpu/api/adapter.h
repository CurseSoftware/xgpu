#pragma once
#include "xgpu/components/components.h"
#include "xgpu/components/metal/adapter_context.h"
#include "xgpu/core/core.h"

namespace xgpu
{
    template <GraphicsApi GAPI = DefaultGraphicsApi>
    // components::detail::IAdapterContext<GAPI> AdapterContext =
    // components::detail::AdapterContext<GAPI>, components::IQueueManager<GAPI>           QueueManager =
    // components::QueueManager<GAPI>>
    // class Adapter : public components::detail::ContextBase<AdapterContext>, public QueueManager
    class Adapter : public components::detail::ContextBase<components::detail::AdapterContext<GAPI>>,
                    public components::QueueManager<GAPI>
    {
      public:
        [[nodiscard]] explicit Adapter(components::detail::AdapterContext<GAPI> ctx) noexcept
            : components::detail::ContextBase<components::detail::AdapterContext<GAPI>>{ std::move(ctx) }
        {
        }
    };
} // namespace xgpu