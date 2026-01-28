#ifndef RHI_VK_PRIV_DEBUG_H
#define RHI_VK_PRIV_DEBUG_H
#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "rhi/expected.h"
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    class DebugMessenger
    {
        public:
            [[nodiscard]] static auto create(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT& info) noexcept -> expected<DebugMessenger, std::string>;

            [[nodiscard]] static auto create_info() -> VkDebugUtilsMessengerCreateInfoEXT;

            auto destroy() noexcept -> void;

        private:
            VkInstance               _instance  { VK_NULL_HANDLE };
            VkDebugUtilsMessengerEXT _messenger { VK_NULL_HANDLE };
    };
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_PRIV_DEBUG_H
