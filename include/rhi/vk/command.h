#ifndef RHI_VK_COMMAND_H
#define RHI_VK_COMMAND_H

#include "rhi/pipeline.h"
#include "rhi/command.h"
#include "rhi/vk/core.h"
#include "rhi/vk/device.h"
#include <cstdint>
#include <optional>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    class CommandBuffer : public ICommandBuffer
    {
        public:
            auto begin() noexcept -> std::optional<Error> override;

            auto beginRenderPass(const RenderPassBeginInfo& info) noexcept -> std::optional<Error> override;
            
            auto bindPipeline(PipelineBindPoint bind_point, const rhi::Pipeline& pipeline) noexcept -> std::optional<Error> override;

            auto setViewport(const ViewportDescription& info) noexcept -> std::optional<Error> override;
            
            auto setScissor(Rect2D scissor) noexcept -> std::optional<Error> override;
            
            auto draw(std::uint32_t num_vertices) noexcept -> std::optional<Error> override;

            auto endRenderPass() noexcept -> std::optional<Error> override;

            auto end() noexcept -> std::optional<Error> override;

            auto memoryBarrier(const TextureBarrierDescription& desc) noexcept -> std::optional<Error> override;
            
            auto copyTextureToBuffer(
                const TextureCopyDescription& texture_copy, 
                Buffer& dest
            ) noexcept -> std::optional<Error> override;

        
        private:
            [[nodiscard]] explicit CommandBuffer(VkDevice device, VkCommandBuffer buffer)
                : _device{ device }
                , _handle{ buffer }
            {}

        private:
            VkDevice _device        { VK_NULL_HANDLE };
            VkCommandBuffer _handle { VK_NULL_HANDLE };

            friend class std::unique_ptr<CommandBuffer>;
            friend class CommandPool;
    };

    class CommandPool : public ICommandPool
    {
        public:
            static auto from_open(rhi::Device& device, const CommandPoolDescription description) noexcept -> expected<rhi::vk::CommandPool, Error>;

        public:
            auto destroy() noexcept -> void override;

            [[nodiscard]] auto allocateBuffers(std::uint32_t num_buffers) noexcept -> expected<std::vector<std::unique_ptr<ICommandBuffer>>, Error> override;

        private:
            [[nodiscard]] explicit CommandPool(VkDevice device) noexcept : _device{ device } {};
        
        private:
            VkDevice _device                  { VK_NULL_HANDLE };
            VkAllocationCallbacks* _allocator { nullptr };
            VkCommandPool _handle             { VK_NULL_HANDLE };
    };
} // namespace rhi::vk

#endif // RHI_VK_COMMAND_H
