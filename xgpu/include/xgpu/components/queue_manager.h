#pragma once
#include "detail/adapter_context.h"
#include "xgpu/core/core.h"
#include "xgpu/types/types.h"

namespace xgpu
{
    template <GraphicsApi GAPI>
    class CommandQueue;
} // namespace xgpu

namespace xgpu::components
{
    template <typename T, GraphicsApi GAPI>
    concept IQueueManager = requires(T manager, QueueTypeFlags queue_type_flags) {
        { manager.create_queue(queue_type_flags) } -> std::same_as<class CommandQueue<GAPI>>;
    } && std::is_default_constructible_v<T>;

    /// @brief The policy oh how queues are managed
    template <GraphicsApi GAPI>
    class QueueManager;
} // namespace xgpu::components