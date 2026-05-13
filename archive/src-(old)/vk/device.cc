#include "xgpu/vk/device.h"
#include "xgpu/vk/instance.h"
#include "xgpu/pipeline_layout.h"
#include "xgpu/core.h"
#include "xgpu/core/log.h"
#include "xgpu/device.h"
#include "xgpu/vk/core.h"
#include "xgpu/types.h"
#include "xgpu/vk/command.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    auto getQueueFamilies(VkPhysicalDevice physical_device) -> std::vector<VkQueueFamilyProperties>
    {
        std::vector<VkQueueFamilyProperties> queue_family_properties {};
        std::uint32_t queue_family_count { 0 };

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
        queue_family_properties.resize(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_properties.data());

        return queue_family_properties;
    }

    auto getComputeFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>
    {
        auto queue_families = getQueueFamilies(physical_device);
        
        for (std::size_t i = 0; i < queue_families.size(); i++)
        {
            if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                return i;
            }
        }

        return std::nullopt;
    }

    auto getGraphicsFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>
    {
        auto queue_families = getQueueFamilies(physical_device);
        
        for (std::size_t i = 0; i < queue_families.size(); i++)
        {
            if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                return i;
            }
        }

        return std::nullopt;
    }
    
    auto getTransferFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>
    {
        auto queue_families = getQueueFamilies(physical_device);
        
        for (std::size_t i = 0; i < queue_families.size(); i++)
        {
            if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                return i;
            }
        }

        return std::nullopt;
    }
    
    auto getPresentFamilyIndex(VkPhysicalDevice physical_device, VkSurfaceKHR surface) -> std::optional<std::uint32_t>
    {
        auto queue_families = getQueueFamilies(physical_device);
        
        for (std::size_t i = 0; i < queue_families.size(); i++)
        {
            VkBool32 present_support { VK_FALSE };
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support);
            if (present_support != VK_FALSE)
            {
                return i;
            }
        }

        return std::nullopt;
    }

    auto getPhysicalDevices(VkInstance instance) noexcept -> std::vector<VkPhysicalDevice>
    {
        std::vector<VkPhysicalDevice> physical_devices {};
        std::uint32_t device_count { 0 };
        
        vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
        physical_devices.resize(device_count);
        vkEnumeratePhysicalDevices(instance, &device_count, physical_devices.data());

        return physical_devices;
    }

    auto getPhysicalDeviceInfo(VkPhysicalDevice physical_device) noexcept -> PhysicalDeviceInfo
    {
        PhysicalDeviceInfo device_info {};

        vkGetPhysicalDeviceFeatures(physical_device, &device_info.features);
        vkGetPhysicalDeviceProperties(physical_device, &device_info.properties);

        device_info.handle = physical_device;
        device_info.graphics_family_index = getGraphicsFamilyIndex(physical_device);
        device_info.transfer_family_index = getTransferFamilyIndex(physical_device);
        device_info.compute_family_index = getComputeFamilyIndex(physical_device);

        return device_info;
    }

    auto Device::create(const vk::DeviceContext& ctx) noexcept -> expected<Device, Error>
    {
        Device device {};
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos {};
        device._physical_device = ctx.physical_device;

        if (!ctx.graphics_queue && !ctx.transfer_queue && !ctx.compute_queue)
        {
            return unexpected( Error("Cannot create vulkan device without any queues.") );
        }

        auto addQueue = [&](DeviceContext::QueueDesc queue) {
            queue_create_infos.emplace_back(VkDeviceQueueCreateInfo {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .queueFamilyIndex = queue.index,
                .queueCount = queue.count,
                .pQueuePriorities = std::addressof(queue.priority)
            });
        };
        auto getQueue = [&](std::uint32_t family_index, std::uint32_t queue_index = 0) {
            Device::Queue device_queue { .family_index = family_index };
            
            vkGetDeviceQueue(device._handle, family_index, queue_index, &device_queue.queue);
            return device_queue;
        };


        // Create the device queues
        {
            std::vector<std::uint32_t> unique_indices {};
            if (ctx.graphics_queue)
            {
                unique_indices.push_back(ctx.graphics_queue->index);
                // addQueue(*ctx.graphics_queue);
            }

            if (ctx.transfer_queue)
            {
                unique_indices.push_back(ctx.transfer_queue->index);
                // addQueue(*ctx.transfer_queue);
            }

            if (ctx.compute_queue)
            {
                unique_indices.push_back(ctx.compute_queue->index);
                // addQueue(*ctx.compute_queue);
            }

            std::sort(unique_indices.begin(), unique_indices.end());
            unique_indices.erase(std::unique(unique_indices.begin(), unique_indices.end()), unique_indices.end());

            for (std::uint32_t index : unique_indices)
            {
                float priority = 1.0f;
                queue_create_infos.emplace_back( VkDeviceQueueCreateInfo {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .pNext = nullptr,
                    .queueFamilyIndex = index,
                    .queueCount = 1,
                    .pQueuePriorities = std::addressof(priority)
                });
            }
        }

        // Create the device itself
        {
            VkPhysicalDeviceFeatures features {};
            VkDeviceCreateInfo create_info {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size()),
                .pQueueCreateInfos = queue_create_infos.data(),
                .enabledExtensionCount = static_cast<std::uint32_t>(ctx.extensions.size()),
                .ppEnabledExtensionNames = ctx.extensions.size() > 1 ? ctx.extensions.data() : nullptr,
                .pEnabledFeatures = &features,
            };

            const VkResult create_result = vkCreateDevice(ctx.physical_device, &create_info, nullptr, &device._handle);
            log::info("Device info");
            if (create_result != VK_SUCCESS)
            {
                return unexpected( Error("Failed to create vulkan device: vkCreateDevice failed") );
            }
        }

        // Get the device queues
        {
            if (ctx.graphics_queue)
            {
                device._graphics_queue = getQueue(ctx.graphics_queue->index);
            }

            if (ctx.transfer_queue)
            {
                device._transfer_queue = getQueue(ctx.transfer_queue->index);
            }

            if (ctx.compute_queue)
            {
                device._compute_queue = getQueue(ctx.compute_queue->index);
            }
        }



        return ok(device);
    }
    auto Device::create_default(const DefaultDeviceContext& ctx) noexcept -> expected<Device, Error>
    {
        auto& vk_handle = *dynamic_cast<vk::Instance*>(ctx.instance.handle());
        auto available_devices = getPhysicalDevices(vk_handle.native_handle());

        // This is largely based on https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_and_queue_families
        // Thanks :)
        auto scoreDevice = [&](const PhysicalDeviceInfo& info) -> int {
            int score { 0 };
            log::debug("Device: {}", info.properties.deviceName);
            
            // Return 0 if none of the required queues are present
            if (ctx.graphics_preference == Preference::Required && !info.graphics_family_index
                || ctx.transfer_preference == Preference::Required && !info.transfer_family_index
                || ctx.compute_preference == Preference::Required && !info.compute_family_index
            ) {
                return 0;
            }

            if (static_cast<std::uint8_t>(ctx.graphics_preference) >= static_cast<std::uint8_t>(Preference::Preferred)
                && info.graphics_family_index
            ) {
                log::debug("\tGraphics family: {}", info.graphics_family_index.value());
                score += 500;
            }

            if (static_cast<std::uint8_t>(ctx.transfer_preference) >= static_cast<std::uint8_t>(Preference::Preferred)
                && info.transfer_family_index
            ) {
                log::debug("\tTransfer family: {}", info.transfer_family_index.value());
                score += 500;
            }

            if (static_cast<std::uint8_t>(ctx.compute_preference) >= static_cast<std::uint8_t>(Preference::Preferred)
                && info.compute_family_index
            ) {
                log::debug("\tCompute family: {}", info.compute_family_index.value());
                score += 500;
            }


            // TODO: add config option for requiring geometry shaders?
            // For now assume that any application preferring graphics requires this
            if (ctx.graphics_preference == Preference::NoPreference && !info.features.geometryShader)
            {
                return 0;
            }

            // Prefer discrete GPU
            if (info.properties.deviceType & VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                score += 1000;
            }

            score += info.properties.limits.maxImageDimension2D;

            return score;
        };

        // Get the capabilities for each physical device
        std::vector<PhysicalDeviceInfo> device_infos {};
        for (const auto& device : available_devices)
        {
            device_infos.push_back(getPhysicalDeviceInfo(device));
        }

        // Sort the devices based on our default scoring system
        std::sort(
            device_infos.begin(), 
            device_infos.end(), 
            [&](const PhysicalDeviceInfo& a, const PhysicalDeviceInfo& b) { return scoreDevice(a) > scoreDevice(b); } 
        );

        auto& best_info = device_infos[0];
        if (scoreDevice(best_info) == 0)
        {
            return unexpected( Error("No suitable vulkan devices found with specified requirements") );
        }
        log::trace("Best device: {} with score of {}", best_info.properties.deviceName, scoreDevice(best_info));

        vk::DeviceContext device_info {
            .physical_device = best_info.handle,
            .physical_device_features = best_info.features
        };

        if (best_info.graphics_family_index)
        {
            log::info("Graphics queue family index: {}", best_info.graphics_family_index.value());
            device_info.graphics_queue = {
                .index = best_info.graphics_family_index.value(),
            };
        }

        if (best_info.transfer_family_index)
        {
            log::info("Transfer queue family index: {}", best_info.transfer_family_index.value());
            device_info.transfer_queue = {
                .index = best_info.transfer_family_index.value(),
            };
        }

        if (best_info.compute_family_index)
        {
            log::info("Compute queue family index: {}", best_info.compute_family_index.value());
            device_info.compute_queue = {
                .index = best_info.compute_family_index.value(),
            };
        }

        return Device::create(device_info);
    }

    auto Device::graphics_queue() const noexcept -> std::optional<std::uint32_t>
    {
        if (!_graphics_queue)
        {
            return std::nullopt;
        }

        return _graphics_queue->family_index;
    }

    auto Device::transfer_queue() const noexcept -> std::optional<std::uint32_t>
    {
        if (!_transfer_queue)
        {
            return std::nullopt;
        }

        return _transfer_queue->family_index;
    }

    auto Device::compute_queue() const noexcept -> std::optional<std::uint32_t>
    {
        if (!_compute_queue)
        {
            return std::nullopt;
        }

        return _compute_queue->family_index;
    }

    auto Device::present_queue() const noexcept -> std::optional<std::uint32_t>
    {
        return std::nullopt;
    }

    auto Device::getMemoryTypeIndex(std::uint32_t type_bits, VkMemoryPropertyFlags flags) const noexcept -> std::optional<std::uint32_t>
    {
        VkPhysicalDeviceMemoryProperties memory_properties {};
        vkGetPhysicalDeviceMemoryProperties(_physical_device, &memory_properties);

        for (std::uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
        {
            if ( (type_bits & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & flags) == flags)
            {
                return i;
            }
        }

        return std::nullopt;
    }

    auto Device::submitSingle(QueueFamilyIndex queue, ICommandBuffer* command_buffer) noexcept -> std::optional<Error>
    {
        auto vk_buffer = dynamic_cast<vk::CommandBuffer*>(command_buffer);
        if (!vk_buffer)
        {
            return Error("failed to get vk::CommandBuffer from xgpu::CommandBuffer");
        }

        auto maybe_queue = [&]() -> std::optional<Queue> {
            if (queue == QueueFamilyIndex::Graphics)
            {
                return _graphics_queue;
            }
            else if (queue == QueueFamilyIndex::Compute)
            {
                return _compute_queue;
            }

            return _transfer_queue;
        }();
        if (!maybe_queue)
        {
            return Error("No vaLid queue for queue family index");
        }

        VkCommandBuffer buffer = vk_buffer->handle();

        VkSubmitInfo submit_info {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers = &buffer,

            // TODO: add semaphore support
        };

        VkFence fence { VK_NULL_HANDLE };
        vkQueueSubmit(maybe_queue.value().queue, 1, &submit_info, fence);

        return std::nullopt;
    }

    auto Device::waitIdle() const noexcept -> void
    {
        vkDeviceWaitIdle(_handle);
    }

    auto Device::destroy() noexcept -> void
    {
        log::trace("Destroying vulkan device...");
        vkDestroyDevice(_handle, nullptr);
        log::trace("Destroyed.");
    }
} // namespace xgpu::vk
