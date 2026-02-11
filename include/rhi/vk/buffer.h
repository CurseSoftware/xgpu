#ifndef RHI_VK_BUFFER_H
#define RHI_VK_BUFFER_H

#include "rhi/buffer.h"
#include "rhi/device.h"
#include "rhi/vk/core.h"
#include <cstddef>
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    class Buffer : public IBuffer
    {
        public:
            static auto create(Device& device, BufferDescription description) noexcept -> expected<vk::Buffer, Error>;

        public:
            auto map(void** dst, std::size_t size) noexcept -> void override;

            auto unmap() noexcept -> void override;

            [[nodiscard]] auto handle() const noexcept -> VkBuffer { return _handle; }

            auto destroy() noexcept -> void override;

        private:
            [[nodiscard]] explicit Buffer(VkDevice device) noexcept
                : _device{ device }
            {}
            
            [[nodiscard]] explicit Buffer(VkDevice device, std::size_t size) noexcept
                : _device{ device }
                , _size{ size }
            {}
        
        private:
            VkDevice _device       { VK_NULL_HANDLE };
            VkBuffer _handle       { VK_NULL_HANDLE };
            VkDeviceMemory _memory { VK_NULL_HANDLE };
            std::size_t _size      { 0 };
    };
} // namespace xgpu::vk

#endif // RHI_VK_BUFFER_H
