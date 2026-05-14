#pragma once
#include "adapter_context.h"
#include "xgpu/core/backends.h"
#include "xgpu/traits/queue_manager.h"

namespace xgpu::traits
{
    template <>
    class QueueManager<GraphicsApi::Metal>
    {
      public:
        template <QueueType Q>
        void
        create_queue()
        {
        }

        void
        create_graphics_queue()
        {
            return create_queue<QueueType::Graphics>();
        }

      private:
    };

    static_assert(IQueueManager<QueueManager<GraphicsApi::Metal>, GraphicsApi::Metal>, "");
} // namespace xgpu::traits