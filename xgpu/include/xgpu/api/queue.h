#pragma once
#include "xgpu/core/core.h"

#include <cstdint>

namespace xgpu
{
    enum class QueueCapabilityFlagBits : std::uint32_t
    {
        Graphics = 0,
        Compute  = 1 << 0,
        Transfer = 1 << 1,
    };

    using QueueCapabilityFlags = Flag<QueueCapabilityFlagBits>;

    struct QueueDescription
    {
        QueueCapabilityFlags capabilities;
    };

    template <GraphicsApi GAPI = DefaultGraphicsApi>
    class CommandQueue
    {
      public:
        CommandQueue() = default;
    };
} // namespace xgpu