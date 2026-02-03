#ifndef RHI_VK_COMMAND_H
#define RHI_VK_COMMAND_H

#include "rhi/command.h"
#include "rhi/vk/core.h"
#include "rhi/vk/device.h"
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    class CommandPool : public ICommandPool
    {
        public:
            static auto create(rhi::vk::Device& device, std::optional<std::uint32_t> family_index = std::nullopt) noexcept -> expected<rhi::vk::CommandPool, Error>;

        public:
            auto destroy() noexcept -> void override;

        private:
            [[nodiscard]] explicit CommandPool(rhi::vk::Device& device) noexcept : _device{ device } {};
        
        private:
            rhi::vk::Device& _device;
            VkAllocationCallbacks* _allocator { nullptr };
            VkCommandPool _handle { VK_NULL_HANDLE };
    };
} // namespace rhi::vk

#endif // RHI_VK_COMMAND_H
