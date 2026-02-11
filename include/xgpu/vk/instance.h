#ifndef RHI_VK_INSTANCE_H
#define RHI_VK_INSTANCE_H

#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "xgpu/platform.h"
#include "xgpu/instance.h"
#include "xgpu/vk/debug.h"
#include "xgpu/vk/core.h"

#include <optional>

namespace xgpu::vk
{
    class [[nodiscard]] Instance : public xgpu::IInstance
    {
        // Factory
        public:
            static auto create(const vk::InstanceContext& ctx) noexcept -> expected<vk::Instance, Error>;

        // API
        public:
            auto destroy() noexcept -> void override;

            auto backend() const noexcept -> Backend override { return Backend::Vulkan; }

            auto native_handle() const noexcept -> VkInstance { return _handle; }
        
        private:
            std::optional<DebugMessenger> _debug_messenger { std::nullopt };
            VkInstance                         _handle     { VK_NULL_HANDLE };
    };
} // namespace xgpu::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_INSTANCE_H
