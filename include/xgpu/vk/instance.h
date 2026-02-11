#ifndef RHI_VK_INSTANCE_H
#define RHI_VK_INSTANCE_H

#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "xgpu/platform.h"
#include "xgpu/vk/debug.h"
#include "xgpu/vk/instance_context.h"
#include "xgpu/error.h"
#include "xgpu/vk/core.h"

#include <optional>

namespace xgpu::vk
{
    class Instance
    {
        // Factory
        public:
            static auto create(const vk::InstanceContext& ctx) noexcept -> expected<vk::Instance, Error>;

        // API
        public:
            auto destroy() noexcept -> void;

            auto backend() const noexcept -> Backend { return Backend::Vulkan; }

            auto native_handle() const noexcept -> VkInstance { return _handle; }
        
        // Private fields
        private:
            std::optional<DebugMessenger> _debug_messenger { std::nullopt };
            VkInstance  _handle                            { VK_NULL_HANDLE };
    };
} // namespace xgpu::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_INSTANCE_H
