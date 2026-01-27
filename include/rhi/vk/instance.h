#ifndef RHI_VK_INSTANCE_H
#define RHI_VK_INSTANCE_H

#ifdef RHI_COMPILE_VULKAN_BACKEND
#include "rhi/instance.h"

namespace rhi::vk
{
    class [[nodiscard]] Instance : public rhi::Instance
    {
        // Factory
        public:
            static auto create(const rhi::vk::InstanceContext& ctx) noexcept -> expected<rhi::vk::Instance, Error>;

        // API
        public:
        
        private:
            using rhi::Instance::Instance;
    };
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_INSTANCE_H
