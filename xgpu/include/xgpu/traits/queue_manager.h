#pragma once
#include "adapter_context.h"
#include "xgpu/core/core.h"

namespace xgpu::traits
{
    template <typename T, GraphicsApi GAPI>
    concept IQueueManager = requires(T manager) {
        { manager.template create_queue<QueueType::Graphics>() } -> std::same_as<void>;
        { manager.template create_queue<QueueType::Transfer>() } -> std::same_as<void>;
        { manager.template create_queue<QueueType::Present>() } -> std::same_as<void>;
        { manager.template create_queue<QueueType::Compute>() } -> std::same_as<void>;

        { manager.create_graphics_queue() } -> std::same_as<void>;
    } && std::is_default_constructible_v<T>;

    /// @brief The policy oh how queues are managed
    template <GraphicsApi GAPI>
    class QueueManager;
} // namespace xgpu::traits