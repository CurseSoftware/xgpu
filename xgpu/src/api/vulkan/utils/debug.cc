#include "debug.h"
#include "xgpu/native/native.h"

#include <iostream>

namespace xgpu::vk
{
    VKAPI_ATTR VkBool32 VKAPI_CALL
    debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
        VkDebugUtilsMessageTypeFlagsEXT             type,
        const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
        void                                       *user_data)
    {
        const std::string_view type_prefix = [type] {
            switch ( type ) {
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

        switch ( severity ) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            std::cout << std::format("INFO: {}{}\n", type_prefix, callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            std::cout << std::format("WARNING: {}{}\n", type_prefix, callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            std::cout << std::format("ERROR: {}{}\n", type_prefix, callback_data->pMessage);
            break;
        default:
            std::cout << std::format("VALIDATION: {}{}\n", type_prefix, callback_data->pMessage);
        }

        return VK_FALSE;
    }

    VkResult
    create_debug_messenger(
        VkInstance                                instance,
        const VkDebugUtilsMessengerCreateInfoEXT &info,
        VkDebugUtilsMessengerEXT                 &messenger,
        const VkAllocationCallbacks              *allocation_callbacks) noexcept
    {
        PFN_vkCreateDebugUtilsMessengerEXT create_func
            = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                instance, "vkCreateDebugUtilsMessengerEXT");
        if ( create_func != nullptr ) {
            return create_func(instance, &info, allocation_callbacks, &messenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void
    destroy_debug_messenger(
        VkInstance                instance,
        VkDebugUtilsMessengerEXT &messenger,
        VkAllocationCallbacks    *allocation_callbacks) noexcept
    {
        PFN_vkDestroyDebugUtilsMessengerEXT destroy_func
            = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                instance, "vkDestroyDebugUtilsMessengerEXT");
        if ( destroy_func != nullptr ) {
            destroy_func(instance, messenger, allocation_callbacks);
        }
    }
} // namespace xgpu::vk