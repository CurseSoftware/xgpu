#include "command.h"
#include "rhi/core/log.h"
#include "rhi/expected.h"
#include "rhi/device.h"
#include "rhi/vk/core.h"
#include "rhi/vk/command.h"
#include "rhi/vk/core.h"
#include <memory>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    auto CommandPool::from_open(rhi::Device &device, const CommandPoolDescription description) noexcept -> expected<CommandPool, Error>
    {
        auto* vk_device = dynamic_cast<vk::Device*>(device.handle());
        if (!vk_device)
        {
            return unexpected( Error("Failed to get vk::Device from rhi::Device") );
        }

        CommandPool pool { vk_device->handle() };

        VkCommandPoolCreateInfo create_info {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = description.family_index
        };

        const VkResult result = vkCreateCommandPool(vk_device->handle(), &create_info, nullptr,  &pool._handle);
        if (result != VK_SUCCESS)
        {
            return unexpected( Error("vkCreateCommandPool != VK_SUCCESS") );
        }

        return ok(pool);
    }

    auto CommandPool::allocateBuffers(std::uint32_t num_buffers) noexcept -> expected<std::vector<std::unique_ptr<ICommandBuffer>>, Error>
    {
        std::vector<VkCommandBuffer> vk_command_buffers(num_buffers);
        std::vector<std::unique_ptr<ICommandBuffer>> command_buffers {};
        
        const VkCommandBufferAllocateInfo allocate_info {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = _handle,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = num_buffers,
        };
        
        const VkResult allocate_result = vkAllocateCommandBuffers(_device, &allocate_info, vk_command_buffers.data());
        if (allocate_result != VK_SUCCESS)
        {
            return unexpected( Error("vkAllocateCommandBuffers != VK_SUCCESS") );
        }

        for (const auto& buffer : vk_command_buffers)
        {
            command_buffers.push_back(std::unique_ptr<vk::CommandBuffer>(new vk::CommandBuffer(_device, buffer)));
        }

        return ok(std::move(command_buffers));
    }

    auto CommandPool::destroy() noexcept -> void
    {
        log::trace("Destroying command pool...");
        vkDestroyCommandPool(_device, _handle, _allocator);
        log::trace("Destroyed.");
    }
} // namespace rhi::vk
