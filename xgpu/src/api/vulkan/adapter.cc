#include "xgpu/api/api.h"
#include "xgpu/core/core.h"
#include "xgpu/native/native.h"

#ifdef XGPU_COMPILE_VULKAN

namespace xgpu
{
    template <>
    void
    vulkan_adapter::destroy() noexcept
    {
        vkDestroyDevice(m_device, nullptr);
    }

} // namespace xgpu

#endif // XGPU_COMPILE_VULKAN