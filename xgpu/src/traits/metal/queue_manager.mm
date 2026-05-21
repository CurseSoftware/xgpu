#include "xgpu/components/components.h"
#include "xgpu/api/api.h"

namespace xgpu::components
{
    CommandQueue<GraphicsApi::Metal> QueueManager<GraphicsApi::Metal>::create_queue(const QueueTypeFlags flags) noexcept
    {
        (void) flags;

        return {};
    }
} // namespace xgpu::components