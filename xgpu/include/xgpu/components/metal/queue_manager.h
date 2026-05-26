#pragma once
#include "../queue_manager.h"
#include "adapter_context.h"
#include "xgpu/components/adapter.h"
#include "xgpu/core/backends.h"
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"
#include "xgpu/types/types.h"

namespace xgpu::components
{
    template <>
    class QueueManager<GraphicsApi::Metal>
    {
      public:
        CommandQueue<GraphicsApi::Metal> create_queue(const QueueTypeFlags flags) noexcept;

      private:
    };
} // namespace xgpu::components