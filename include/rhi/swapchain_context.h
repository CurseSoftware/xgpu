#ifndef RHI_SWAPCHAIN_CONTEXT_H
#define RHI_SWAPCHAIN_CONTEXT_H

#include "rhi/vk/swapchain_context.h"
#include <variant>
namespace xgpu
{
    struct OpenSwapChainContext
    {

    };

    using SwapChainContext = std::variant<
        OpenSwapChainContext
#ifdef RHI_COMPILE_VULKAN_BACKEND
        , vk::SwapChainContext
#endif // RHI_COMPILE_VULKAN_BACKEND
    >;
} // namespace xgpu

#endif // RHI_SWAPCHAIN_CONTEXT_H
