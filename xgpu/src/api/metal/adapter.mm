#include "xgpu/api/api.h"
#include "xgpu/core/core.h"
#include "metal_headers.h"

#ifdef XGPU_COMPILE_METAL

namespace xgpu
{
    template <>
    void Adapter<GraphicsApi::Metal>::destroy() noexcept
    {
        [m_device release];
    }
} // namespace xgpu

#endif // XGPU_COMPILE_METAL
