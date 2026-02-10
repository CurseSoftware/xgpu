#ifndef RHI_COMMAND_H
#define RHI_COMMAND_H

#include "rhi/buffer.h"
#include "rhi/device.h"
#include "rhi/error.h"
#include "rhi/expected.h"
#include "rhi/framebuffer.h"
#include "rhi/image_view.h"
#include "rhi/pipeline.h"
#include "rhi/renderpass.h"
#include "rhi/types.h"
#include <memory>
#include <optional>

namespace rhi
{
    struct RenderPassBeginInfo 
    {
        Renderpass& renderpass;
        Framebuffer& framebuffer;
        
        struct
        {
            struct
            {
                std::int32_t x { 0 };
                std::int32_t y { 0 };
            } offset {};

            Extent2D extent {};
        } render_area {};
        
        std::optional<ClearColorValue> clear_color { std::nullopt };
        std::optional<DepthStencilClearValue> depth_stencil_clear { std::nullopt };
    };

    struct TextureCopyDescription
    {
        ImageView& image;
        std::uint32_t mip_level { 0 };
        std::uint32_t base_array_layer { 0 };
        Extent3D extent {};
    };

    struct TextureBarrierDescription
    {
        ImageView& texture;
        PipelineStage src_stage {};
        PipelineStage dst_stage {};
        std::uint32_t mip_level { 0 };
        std::uint32_t base_array_layer { 0 };
        Extent3D extent {};
    };

    class ICommandBuffer
    {
        public:
            virtual auto begin()                                          noexcept -> std::optional<Error> = 0;
            virtual auto beginRenderPass(const RenderPassBeginInfo&)      noexcept -> std::optional<Error> = 0;
            virtual auto bindPipeline(PipelineBindPoint, const Pipeline&) noexcept -> std::optional<Error> = 0;
            virtual auto setViewport(const ViewportDescription& info)     noexcept -> std::optional<Error> = 0;
            virtual auto setScissor(Rect2D scissor)                       noexcept -> std::optional<Error> = 0;
            virtual auto draw(std::uint32_t num_vertices)                 noexcept -> std::optional<Error> = 0;
            virtual auto endRenderPass()                                  noexcept -> std::optional<Error> = 0;
            virtual auto end()                                            noexcept -> std::optional<Error> = 0;

            virtual auto memoryBarrier(const TextureBarrierDescription&)  noexcept -> std::optional<Error> = 0;

            // Copy the data from a framebuffer (texture) to a Buffer object
            virtual auto copyTextureToBuffer(
                const TextureCopyDescription& texture_copy, 
                Buffer& dest
            ) noexcept -> std::optional<Error> = 0;
    };
    
    class CommandBuffer : public ICommandBuffer
    {
        // API
        public:
            auto begin() noexcept -> std::optional<Error> override { return _handle->begin(); }

            auto beginRenderPass(const RenderPassBeginInfo& info) noexcept -> std::optional<Error> override { return _handle->beginRenderPass(info); }
            
            auto bindPipeline(PipelineBindPoint bind_point, const Pipeline& pipeline) noexcept -> std::optional<Error> override { return _handle->bindPipeline(bind_point, pipeline); }

            auto setViewport(const ViewportDescription& info) noexcept -> std::optional<Error> override { return _handle->setViewport(info); }

            auto setScissor(Rect2D scissor) noexcept -> std::optional<Error> override { return _handle->setScissor(scissor); }
            
            auto draw(std::uint32_t num_vertices) noexcept -> std::optional<Error> override { return _handle->draw(num_vertices); }

            auto endRenderPass() noexcept -> std::optional<Error> override { return _handle->endRenderPass(); }

            auto end() noexcept -> std::optional<Error> override { return _handle->end(); }

            auto memoryBarrier(const TextureBarrierDescription& desc) noexcept -> std::optional<Error> override { return _handle->memoryBarrier(desc); }
            
            auto copyTextureToBuffer(
                const TextureCopyDescription& texture_copy, 
                Buffer& dest
            ) noexcept -> std::optional<Error> override
            {
                return _handle->copyTextureToBuffer(texture_copy, dest);
            }

        // Private special members
        private:
            [[nodiscard]] explicit CommandBuffer() noexcept = default;

        // Private fields
        private:
            std::unique_ptr<ICommandBuffer> _handle { nullptr };
    };

    struct CommandPoolDescription
    {
        std::uint32_t family_index {};
    };
    
    class ICommandPool
    {
        public:
            virtual auto destroy() noexcept -> void = 0;

            // Allocate 
            virtual auto allocateBuffers(std::uint32_t num_buffers) noexcept -> expected<std::vector<std::unique_ptr<ICommandBuffer>>, Error> = 0;
    };
    
    class CommandPool : public ICommandPool
    {
        public:
            // Create a command pool from the device
            // NOTE: we prefer copy over reference here since Description is lightweight
            [[nodiscard]] static auto create(rhi::Device& device, const CommandPoolDescription description) noexcept -> expected<CommandPool, Error>;

            // Destroy the command pool
            auto destroy() noexcept -> void { _handle->destroy(); }

            auto allocateBuffers(std::uint32_t num_buffers) noexcept -> expected<std::vector<std::unique_ptr<ICommandBuffer>>, Error> { return std::move(_handle->allocateBuffers(num_buffers)); }

        private:
            std::unique_ptr<ICommandPool> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_COMMAND_H
