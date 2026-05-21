#pragma once
#include <cstdint>

namespace xgpu
{
    /// @brief The types of command queues that can be created
    enum class QueueType : std::uint32_t
    {
        Graphics = 0x00000001,
        Compute  = 0x00000002,
        Transfer = 0x00000004,
        Present  = 0x00000008,
    };

    /// @brief Flags specifying queue capabilities
    using QueueTypeFlags = Flag<QueueType>;
}