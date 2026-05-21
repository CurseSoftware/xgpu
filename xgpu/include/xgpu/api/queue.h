#pragma once
#include "xgpu/core/core.h"

namespace xgpu
{
    template <GraphicsApi GAPI = DefaultGraphicsApi>
    class CommandQueue
    {
      public:
        CommandQueue() = default;
    };
} // namespace xgpu