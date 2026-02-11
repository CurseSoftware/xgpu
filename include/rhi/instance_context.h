#ifndef RHI_INSTANCE_CONTEXT_H
#define RHI_INSTANCE_CONTEXT_H

#include "vk/instance_context.h"
#include "dx12/instance_context.h"

#include <variant>

namespace xgpu
{
    // The non-api-specific options for configuring an instance
    struct DefaultInstanceContext 
    {
    };
    
    using InstanceContext = 
        std::variant<
            DefaultInstanceContext
#ifdef RHI_COMPILE_VULKAN_BACKEND
            , vk::InstanceContext
#endif // RHI_COMPILE_VULKAN_BACKEND

#ifdef RHI_COMPILE_DX12_BACKEND
            , dx12::InstanceContext
#endif // RHI_COMPILE_DX12_BACKEND
        >;
} // namespace xgpu

#endif // RHI_INSTANCE_CONTEXT_H
