#include "xgpu/data/window.h"
#include "xgpu/core/log.h"
#include "xgpu/expected.h"
#include "xgpu/platform.h"
#include "xgpu/vk/core.h"
#include "xgpu/vk/debug.h"
#include "xgpu/vk/instance_context.h"
#include "vk/validation.h"
#include "vk/extension.h"
#include "xgpu/vk/instance.h"

#include <vector>

namespace xgpu::vk
{
    auto Instance::create(const vk::InstanceContext& ctx) noexcept -> expected<Instance, Error>
    {
        Instance inst = Instance();
        VkDebugUtilsMessengerCreateInfoEXT debug_create_info = DebugMessenger::create_info();

        priv::ValidationLayerManager validation_handler {};
        std::vector<priv::RequestedLayer> requested_layers {};

        priv::ExtensionHandler instance_extension_handler {};
        std::vector<priv::RequestedExtension> requested_extensions {};
        instance_extension_handler.fetchAll<priv::ExtensionSource::Instance>();

        const bool use_debug
            = ctx.enable_debug
            && instance_extension_handler.isSupported(vk::DEBUG_EXTENSION_NAME);

        if (use_debug)
        {
            log::debug("Enabling Vulkan debug capabilities");
            requested_extensions.emplace_back(true, vk::DEBUG_EXTENSION_NAME);
            requested_layers.emplace_back(true, vk::DEBUG_LAYER_NAME);
        }

        requested_extensions.emplace_back(true, VK_KHR_SURFACE_EXTENSION_NAME);
        requested_extensions.emplace_back(true, PLATFORM_SURFACE_NAME);

        const auto expected_validation_layers = validation_handler.getRequestedLayers(requested_layers);
        const auto expected_extensions = instance_extension_handler.getRequested(requested_extensions);

        if (!expected_validation_layers.has_value())
        {
            return unexpected( Error(expected_validation_layers.unwrap_error()) );
        }
        
        if (!expected_extensions.has_value())
        {
            log::error("Extension not found");
            return unexpected( Error(expected_extensions.unwrap_error()) );
        }

        const auto validation_layers = expected_validation_layers.unwrap();
        const auto extensions = expected_extensions.unwrap();

        const VkApplicationInfo app_info {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "RHI_VK_APPLICATION",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "RHI_VK_ENGINE",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
        };

        const VkInstanceCreateInfo instance_info {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = use_debug ? &debug_create_info : nullptr,
            .pApplicationInfo = &app_info,
            .enabledLayerCount = static_cast<std::uint32_t>(validation_layers.size()),
            .ppEnabledLayerNames = validation_layers.data(),
            .enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data()
        };

        const VkResult create_result = vkCreateInstance(&instance_info, nullptr, &inst._handle);
        if (create_result != VK_SUCCESS)
        {
            return unexpected( Error("Failed to create instance. vkCreateInstance call != VK_SUCCESS") );
        }

        if (use_debug)
        {
            auto debug_messenger_exp = DebugMessenger::create(inst._handle, debug_create_info);
            if (!debug_messenger_exp.has_value())
            {
                return unexpected( Error(debug_messenger_exp.unwrap_error()) );
            }
            auto debug_messenger = debug_messenger_exp.unwrap();
            inst._debug_messenger = debug_messenger;
        }
        

        return ok(inst);
    }

    auto Instance::destroy() noexcept -> void
    {
        if (_debug_messenger)
        {
            log::trace("Destroying vulkan debug messenger...");
            _debug_messenger->destroy();
            log::trace("Debug messenger destroyed.");
        }
        
        log::trace("Destroying vulkan instance...");
        vkDestroyInstance(_handle, nullptr);
        log::trace("Destroyed.");
    }

} // namespace xgpu::vk
