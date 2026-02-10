#include "rhi/vk/buffer.h"
#include "core/log.h"
#include "rhi/vk/device.h"
#include "types.h"
#include "vk/vk_utils.h"
#include <cstring>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    auto Buffer::create(rhi::Device& device, BufferDescription description) noexcept -> expected<vk::Buffer, Error>
    {
        auto vk_device = device.get_as<vk::Device>();
        if (!vk_device)
        {
            return unexpected( Error("failed to get vk::Device from rhi::Device") );
        }
        vk::Buffer buffer { vk_device->handle() , description.size};

        VkMemoryPropertyFlags memory_properties {
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };
        if (description.mapping == BufferMapping::MapRead)
        {
            memory_properties |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
        }

        VkBufferCreateInfo create_info {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = description.size,
            .usage = convertBufferUsage(description.usage),
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        const VkResult create_result = vkCreateBuffer(
            vk_device->handle(), 
            &create_info, 
            nullptr, 
            &buffer._handle
        );
        if (create_result != VK_SUCCESS)
        {
            return unexpected( Error("vkCreateBuffer != VK_SUCCESS") );
        }

        VkMemoryRequirements requirements {};
        vkGetBufferMemoryRequirements(vk_device->handle(), buffer._handle, &requirements);

        VkMemoryAllocateInfo alloc_info {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = requirements.size,
            .memoryTypeIndex = vk_device->getMemoryTypeIndex(requirements.memoryTypeBits, memory_properties).value()
        };

        const VkResult alloc_result = vkAllocateMemory(vk_device->handle(), &alloc_info, nullptr, &buffer._memory);
        if (alloc_result != VK_SUCCESS)
        {
            return unexpected( Error("vkAllocateMemory != VK_SUCCESS") );
        }

        vkBindBufferMemory(vk_device->handle(), buffer._handle, buffer._memory, /* offset */ 0);

        return ok(buffer);
    }

    auto Buffer::map(void* data, std::size_t size) noexcept -> void
    {
        vkMapMemory(_device, _memory, 0, size, 0, &data);
    }

    auto Buffer::unmap() noexcept -> void
    {
        vkUnmapMemory(_device, _memory);
    }

    auto Buffer::destroy() noexcept -> void
    {
        log::trace("Freeing buffer memory...");
        vkFreeMemory(_device, _memory, nullptr);
        log::trace("Freed.");
        log::trace("Destroying buffer...");
        vkDestroyBuffer(_device, _handle, nullptr);
        log::trace("Destroyed.");
    }
} // namespace rhi::vk
