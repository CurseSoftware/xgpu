#include "utils/utils.h"
#include "xgpu/api/api.h"
#include "xgpu/native/native.h"

#include <iostream>

namespace xgpu
{
    template <>
    expected<Instance<GraphicsApi::Vulkan>, Error>
    vulkan_instance::create(const InstanceDesc &desc) noexcept
    {
        vulkan_instance         instance{};
        const VkApplicationInfo app_info = {
            .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName   = "XGPU_VULKAN_APPLICATION",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName        = "XGPU_VULKAN_ENGINE_NAME",
            .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion         = VK_API_VERSION_1_0,
        };

        std::vector<vk::RequestedExtension>               requested_extensions{};
        vk::ExtensionHandler<vk::ExtensionKind::Instance> instance_extension_handler{};

        const char   *validationLayers[] = { "VK_LAYER_KHRONOS_validation" };
        std::uint32_t flags              = 0;

        if constexpr ( is_molten_vk() ) {
            flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            requested_extensions.emplace_back(
                vk::RequestedExtension{ .required = true, .name = "VK_KHR_portability_enumeration" });
        }

        expected<std::vector<const char *>, Error> extension_names
            = instance_extension_handler.get_requested(requested_extensions);

        if ( !extension_names ) {
            return unexpected(extension_names.error());
        }

        const VkInstanceCreateInfo create_info = {
            .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
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

        return instance;
    }
} // namespace xgpu