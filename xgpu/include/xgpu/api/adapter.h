#pragma once
#include "xgpu/components/components.h"
#include "xgpu/components/metal/adapter_context.h"
#include "xgpu/core/core.h"

namespace xgpu
{
    template <GraphicsApi GAPI = DefaultGraphicsApi>
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