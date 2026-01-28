#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "rhi/core/log.h"
#include "rhi/vk/debug.h"
#include "rhi/expected.h"

#include <iostream>
#include <string_view>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL g_debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data
    ) {
        const std::string_view type_prefix = [type] {
            switch (type)
            {
                case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                    return "<GENERAL>     :";
                case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                    return "<VALIDATION>  :";
                case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                    return "<PERFORMANCE> :";
                default:
                    return "<UNKNOWN>     :";
            }
        }();

        // TODO: use user_data with a callback to handle the message
        switch (severity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                log::info("{}{}", type_prefix, callback_data->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                log::trace("{}{}", type_prefix, callback_data->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                log::warning("{}{}", type_prefix, callback_data->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                log::error("{}{}", type_prefix, callback_data->pMessage);
                break;
            default:
                log::debug("{}{}", type_prefix, callback_data->pMessage);
        }

        return VK_FALSE;
    }

    auto create_debug_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT& info, VkDebugUtilsMessengerEXT& messenger, VkAllocationCallbacks* allocation_callbacks = nullptr) -> VkResult
    {
        auto create_func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (create_func != nullptr)
        {
            return create_func(instance, &info, allocation_callbacks, &messenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    auto destroy_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT& messenger, VkAllocationCallbacks* allocation_callbacks = nullptr) noexcept -> void
    {
        auto destroy_func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (destroy_func != nullptr)
        {
            destroy_func(instance, messenger, allocation_callbacks);
        }
    }

    auto DebugMessenger::create_info() -> VkDebugUtilsMessengerCreateInfoEXT
    {
        return VkDebugUtilsMessengerCreateInfoEXT {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = 
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = 
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = g_debug_callback,

            // TODO: use this to configure logging
            .pUserData = nullptr
        };
    }

    auto DebugMessenger::create(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT &info) noexcept -> expected<DebugMessenger, std::string>
    {
        DebugMessenger messenger {};
        messenger._instance = instance;
        
        const VkResult result = create_debug_messenger(instance, info, messenger._messenger);

        return ok(messenger);
    }

    auto DebugMessenger::destroy() noexcept -> void
    {
        destroy_debug_messenger(_instance, _messenger);
    }
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
