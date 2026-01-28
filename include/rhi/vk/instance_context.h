#ifndef RHI_VK_INSTANCE_CONTEXT_H
#define RHI_VK_INSTANCE_CONTEXT_H
#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "rhi/data/window.h"

#include <functional>
#include <vector>

namespace rhi::vk
{
    struct InstanceContext
    {
        bool enable_debug { false };
        data::SurfaceData surface_data { data::HeadlessSurfaceData{} };
    };
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_INSTANCE_CONTEXT_H
