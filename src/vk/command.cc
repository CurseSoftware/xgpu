#include "expected.h"
#include "rhi/vk/core.h"
#include "rhi/vk/command.h"
#include <cmath>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    auto CommandPool::create(rhi::vk::Device &device, std::optional<std::uint32_t> family_index) noexcept -> expected<CommandPool, Error>
    {
        CommandPool pool { device };

        std::optional<std::uint32_t> queue_family_index = family_index
            ? *family_index
            : device.graphicsFamilyIndex()
            ;

        if (!queue_family_index)
        {
            return unexpected( Error("No valid queue family index supplied or found.") );
        }

        VkCommandPoolCreateInfo create_info {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = *family_index 
        };

        const VkResult result = vkCreateCommandPool(device.handle(), &create_info, nullptr,  &pool._handle);
        if (result != VK_SUCCESS)
        {
            return unexpected( Error("Failed to create command pool. vkCreateCommandPool != VK_SUCCESS") );
        }

        return ok(pool);
    }

    auto CommandPool::destroy() noexcept -> void
    {
        vkDestroyCommandPool(_device.handle(), _handle, _allocator);
    }
} // namespace rhi::vk
