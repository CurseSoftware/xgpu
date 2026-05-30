#include "utils/device.h"
#include "utils/utils.h"
#include "xgpu/api/api.h"
#include "xgpu/native/native.h"

#include <iostream>
#include <optional>
#include <set>
#include <unordered_set>

namespace xgpu
{
    template <>
    std::span<data::PhysicalDevice>
    vulkan_instance::enumerate_devices() noexcept
    {
        if ( m_physical_devices.empty() ) {
            VkPhysicalDevice *dummy_devices = nullptr;
            std::uint32_t     count         = 0;

            vkEnumeratePhysicalDevices(m_instance, std::addressof(count), dummy_devices);
            std::vector<VkPhysicalDevice> devices(count);
            vkEnumeratePhysicalDevices(m_instance, std::addressof(count), devices.data());

            auto transformed
                = devices | std::views::transform([](const VkPhysicalDevice &physical_device) {
                      VkPhysicalDeviceProperties       properties{};
                      VkPhysicalDeviceMemoryProperties memory_properties{};
                      vkGetPhysicalDeviceProperties(physical_device, &properties);
                      vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

                      // TODO: we need to reconcile how we cound video memory for physical devices
                      VkDeviceSize total_vram = 0;
                      for ( std::uint32_t i = 0; i < memory_properties.memoryHeapCount; i++ ) {
                          if ( memory_properties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT ) {
                              total_vram += memory_properties.memoryHeaps[i].size;
                          }
                      }

                      return data::PhysicalDevice{
                          .name            = properties.deviceName,
                          .type            = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
                                                 ? data::PhysicalDeviceType::DiscreteGPU
                                                 : data::PhysicalDeviceType::IntegratedGPU,
                          .video_ram_bytes = static_cast<std::uint32_t>(total_vram)
                      };
                  });

            m_physical_devices = std::vector(transformed.begin(), transformed.end());
        }

        return m_physical_devices;
    }

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
                vk::RequestedExtension{ .required = true,
                                        .name     = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME });
            requested_extensions.emplace_back(
                vk::RequestedExtension{ .required = true, .name = "VK_EXT_metal_surface" });
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
            return unexpected(Error::make_error(ErrorCode::InstanceError, "vkCreateInstance != VK_SUCCESS"));
        }

        if ( debug_create_info ) {
            VkDebugUtilsMessengerEXT debug_messenger{};
            vk::create_debug_messenger(instance.m_instance, *debug_create_info, debug_messenger);
            instance.m_debug_manager = debug_messenger;
        }

        // Fill the instance's physical devices
        (void)instance.enumerate_devices();

        return instance;
    }

    template <>
    data::PhysicalDevice
    vulkan_instance::default_physical_device() const noexcept
    {
        return m_physical_devices[0];
    }

    template <>
    expected<Adapter<GraphicsApi::Vulkan>, Error>
    vulkan_instance::create_adapter(std::optional<data::PhysicalDevice> physical_device) noexcept
    {
        data::PhysicalDevice selected_physical_device = default_physical_device();
        VkDevice             device{ VK_NULL_HANDLE };

        if ( physical_device )
            selected_physical_device = physical_device.value();

        std::optional<vk::PhysicalDeviceProperties>     found_device{};
        const std::vector<vk::PhysicalDeviceProperties> available_devices
            = vk::enumerate_physical_devices(m_instance);
        for ( const vk::PhysicalDeviceProperties &properties : available_devices ) {
            if ( properties.properties.deviceName == selected_physical_device.name ) {
                found_device = properties;
            }
        }

        if ( !found_device ) {
            return unexpected(
                Error::make_error(
                    ErrorCode::DeviceNotFound,
                    std::format("No device ({}) was found.", physical_device->name)));
        }

        std::unordered_set<std::uint32_t> queue_family_indices{};
        if ( const std::optional graphics_index
             = vk::get_queue_family_index<VK_QUEUE_GRAPHICS_BIT>(found_device->device) ) {
            queue_family_indices.insert(*graphics_index);
        }

        if ( const std::optional transfer_index
             = vk::get_queue_family_index<VK_QUEUE_TRANSFER_BIT>(found_device->device) ) {
            queue_family_indices.insert(*transfer_index);
        }

        if ( const std::optional compute_index
             = vk::get_queue_family_index<VK_QUEUE_COMPUTE_BIT>(found_device->device) ) {
            queue_family_indices.insert(*compute_index);
        }

        auto queue_infos = queue_family_indices | std::views::transform([](std::uint32_t index) {
                               float priority = 1.0f;
                               return VkDeviceQueueCreateInfo{
                                   .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                   .queueFamilyIndex = index,
                                   .queueCount       = 1,
                                   .pQueuePriorities = std::addressof(priority),
                               };
                           });
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos{ queue_infos.begin(), queue_infos.end() };

        {
            vk::ExtensionHandler<vk::ExtensionKind::Device> device_extensions{ found_device->device };
            std::vector<vk::RequestedExtension>             requested_extensions{
                vk::RequestedExtension{ .name = VK_KHR_SWAPCHAIN_EXTENSION_NAME, .required = true },
            };

            if constexpr ( is_molten_vk() ) {
                requested_extensions.emplace_back(
                    vk::RequestedExtension{ .name = "VK_KHR_portability_subset", .required = true });
            }

            expected<std::vector<const char *>, Error> extension_names
                = device_extensions.get_requested(requested_extensions);

            if ( !extension_names ) {
                return unexpected(extension_names.error());
            }

            VkPhysicalDeviceFeatures device_features{};
            VkDeviceCreateInfo       create_info{
                      .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                      .queueCreateInfoCount    = static_cast<std::uint32_t>(queue_create_infos.size()),
                      .pQueueCreateInfos       = queue_create_infos.data(),
                      .pEnabledFeatures        = std::addressof(device_features),
                      .enabledExtensionCount   = static_cast<std::uint32_t>(extension_names->size()),
                      .ppEnabledExtensionNames = extension_names->data(),
            };

            const vk::Result result = vk::create_device(found_device->device, create_info, device);
            if ( result != VK_SUCCESS ) {
                return unexpected(
                    Error::make_error(
                        ErrorCode::AdapterCreationFailed,
                        std::format("vkCreateDeviceFailed with error: {}", result.to_string())));
            }
        }

        return vulkan_adapter(device, selected_physical_device);
    }

    template <>
    void
    vulkan_instance::destroy() noexcept
    {
        if ( m_debug_manager ) {
            std::cout << "Destroying debug... ";
            vk::destroy_debug_messenger(m_instance, *m_debug_manager);
            std::cout << "done.\n";
        }

        std::cout << "Destroying instance... ";
        vkDestroyInstance(m_instance, nullptr);
        std::cout << "done.\n";
    }
} // namespace xgpu