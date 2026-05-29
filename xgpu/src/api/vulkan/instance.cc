#include "utils/utils.h"
#include "xgpu/api/api.h"
#include "xgpu/native/native.h"

#include <iostream>
#include <optional>

namespace xgpu
{
    template <>
    expected<Instance<GraphicsApi::Vulkan>, Error>
    vulkan_instance::create(const InstanceDesc &desc) noexcept
    {
        vulkan_instance             instance{};
        constexpr VkApplicationInfo app_info = {
            .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName   = "XGPU_VULKAN_APPLICATION",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName        = "XGPU_VULKAN_ENGINE_NAME",
            .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion         = VK_API_VERSION_1_0,
        };

        std::vector<vk::RequestedExtension>               requested_extensions{};
        std::vector<vk::RequestedValidationLayer>         requested_validation_layers{};
        vk::ExtensionHandler<vk::ExtensionKind::Instance> instance_extension_handler{};
        std::uint32_t                                     flags = 0;
        std::optional<VkDebugUtilsMessengerCreateInfoEXT> debug_create_info{ std::nullopt };

        if ( desc.validation_enabled ) {
            requested_extensions.emplace_back(
                vk::RequestedExtension{ .name = VK_EXT_DEBUG_UTILS_EXTENSION_NAME, .required = true });
            requested_validation_layers.emplace_back(
                vk::RequestedValidationLayer{ .name = "VK_LAYER_KHRONOS_validation", .required = true });

            debug_create_info = VkDebugUtilsMessengerCreateInfoEXT{
                .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                   | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                               | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                               | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                .pfnUserCallback = vk::debug_callback,

                // TODO: use this to configure logging
                .pUserData = nullptr
            };
        }

        if constexpr ( is_molten_vk() ) {
            flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            requested_extensions.emplace_back(
                vk::RequestedExtension{ .required = true, .name = "VK_KHR_portability_enumeration" });
        }

        expected<std::vector<const char *>, Error> extension_names
            = instance_extension_handler.get_requested(requested_extensions);

        if ( !extension_names ) {
            return unexpected(
                Error::make_error(
                    ErrorCode::InstanceError,
                    std::format(
                        "Required Vulkan extension not found: {}", extension_names.error().to_string())));
        }

        const VkInstanceCreateInfo create_info = {
            .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext                   = debug_create_info ? std::addressof(*debug_create_info) : nullptr,
            .pApplicationInfo        = std::addressof(app_info),
            .enabledExtensionCount   = static_cast<std::uint32_t>(extension_names->size()),
            .ppEnabledExtensionNames = extension_names->data(),
            .flags                   = flags,
        };

        const VkResult result
            = vkCreateInstance(std::addressof(create_info), nullptr, std::addressof(instance.m_instance));

        if ( result != VK_SUCCESS ) {
            std::cout << "VULKAN ERROR: " << vk::to_string(result) << std::endl;
            return unexpected(Error::make_error(ErrorCode::InstanceError, "vkCreateInstance != VK_SUCCESS"));
        }

        if ( debug_create_info ) {
            VkDebugUtilsMessengerEXT debug_messenger{};
            vk::create_debug_messenger(instance.m_instance, *debug_create_info, debug_messenger);
            instance.m_debug_manager = debug_messenger;
        }

        return instance;
    }

    template <>
    void
    vulkan_instance::destroy() noexcept
    {
        if ( m_debug_manager ) {
            vk::destroy_debug_messenger(m_instance, *m_debug_manager);
        }

        vkDestroyInstance(m_instance, nullptr);
    }
} // namespace xgpu