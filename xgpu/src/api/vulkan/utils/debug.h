#pragma once
#include "xgpu/native/native.h"

namespace xgpu::vk
{
    /// @brief Debug callback
    VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
        VkDebugUtilsMessageTypeFlagsEXT             type,
        const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
        void                                       *user_data);

    /// @brief Create a debug messenger
    VkResult create_debug_messenger(
        VkInstance                                instance,
        const VkDebugUtilsMessengerCreateInfoEXT &info,
        VkDebugUtilsMessengerEXT                 &messenger,
        const VkAllocationCallbacks              *allocation_callbacks = nullptr) noexcept;

    /// @brief Destroy a debug messenger
    void destroy_debug_messenger(
        VkInstance                instance,
        VkDebugUtilsMessengerEXT &messenger,
        VkAllocationCallbacks    *allocation_callbacks = nullptr) noexcept;
} // namespace xgpu::vk