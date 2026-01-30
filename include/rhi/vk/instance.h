#ifndef RHI_VK_INSTANCE_H
#define RHI_VK_INSTANCE_H

#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "rhi/platform.h"
#include "rhi/instance.h"
#include "rhi/vk/debug.h"

#include <optional>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    class [[nodiscard]] Instance : public rhi::IInstance
    {
        // Factory
        public:
            static auto create(const rhi::vk::InstanceContext& ctx) noexcept -> expected<rhi::vk::Instance, Error>;

        // API
        public:
            auto destroy() noexcept -> void override;

            auto backend() const noexcept -> Backend override { return Backend::Vulkan; }

            auto native_handle() const noexcept -> VkInstance { return _handle; }
        
        private:
            std::optional<DebugMessenger> _debug_messenger { std::nullopt };
            VkInstance                         _handle     { VK_NULL_HANDLE };
    };
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_INSTANCE_H
