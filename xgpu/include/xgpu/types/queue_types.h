#pragma once
#include <cstdint>

namespace xgpu
{
    enum class QueueType : std::uint32_t
    {
        Graphics = 0,
        Compute,
        Transfer,
        Present,
    };
}