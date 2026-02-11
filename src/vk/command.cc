#include "xgpu/command.h"
#include "xgpu/pipeline.h"
#include "xgpu/core/log.h"
#include "xgpu/expected.h"
#include "xgpu/device.h"
#include "xgpu/vk/core.h"
#include "xgpu/vk/command.h"
#include "xgpu/vk/framebuffer.h"
#include "xgpu/vk/core.h"
#include "xgpu/vk/buffer.h"
#include "xgpu/vk/image_view.h"
#include "xgpu/vk/pipeline.h"
#include "xgpu/vk/renderpass.h"
#include "vk/vk_utils.h"
#include <cstddef>
#include <memory>
#include <optional>
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    auto CommandPool::from_open(xgpu::Device &device, const CommandPoolDescription description) noexcept -> expected<CommandPool, Error>
    {
        auto* vk_device = dynamic_cast<vk::Device*>(device.handle());
        if (!vk_device)
        {
            return unexpected( Error("Failed to get vk::Device from xgpu::Device") );
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

    auto CommandBuffer::begin() noexcept -> std::optional<Error>
    {
        VkCommandBufferBeginInfo begin_info {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = 0,
            .pInheritanceInfo = nullptr,
        };

        const VkResult begin_result = vkBeginCommandBuffer(_handle, &begin_info);
        if (begin_result != VK_SUCCESS)
        {
            return Error("vkBeginCommandBuffer != VK_SUCCESS");
        }

        return std::nullopt;
    }

    auto CommandBuffer::beginRenderPass(const RenderPassBeginInfo& info) noexcept -> std::optional<Error>
    {
        auto vk_renderpass = dynamic_cast<vk::Renderpass*>(info.renderpass.handle());
        if (!vk_renderpass)
        {
            return Error("failed to get vk::RenderPass from xgpu::RenderPass");
        }
        
        auto vk_framebuffer = info.framebuffer.get_as<vk::Framebuffer>();
        if (!vk_framebuffer)
        {
            return Error("failed to get vk::RenderPass from xgpu::RenderPass");
        }

        std::size_t clears_offset { 0 };
        std::uint32_t num_clears { 0 };
        std::array<VkClearValue, 2> clear_values {};

        if (info.clear_color)
        {
            num_clears++;
            clear_values[0].color.uint32[0] = info.clear_color->r;
            clear_values[0].color.uint32[1] = info.clear_color->g;
            clear_values[0].color.uint32[2] = info.clear_color->r;
            clear_values[0].color.uint32[3] = info.clear_color->a;
        }
        else
        {
            clears_offset = 1;
        }

        if (info.depth_stencil_clear)
        {
            num_clears++;
            clear_values[1].depthStencil = {
                .depth = info.depth_stencil_clear->depth,
                .stencil = info.depth_stencil_clear->stencil,
            };
        }

        VkRenderPassBeginInfo renderpass_begin_info {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = vk_renderpass->handle(),
            .framebuffer = vk_framebuffer->handle(),
            .renderArea = {
                .offset = {
                    .x = info.render_area.offset.x,
                    .y = info.render_area.offset.y,
                },
                .extent = {
                    .width = info.render_area.extent.width,
                    .height = info.render_area.extent.height,
                },
            },
            .clearValueCount = num_clears,
            .pClearValues = num_clears == 0
                ? nullptr
                : std::span<VkClearValue>(clear_values).subspan(clears_offset, num_clears).data()
                ,
        };

        vkCmdBeginRenderPass(
            _handle, 
            &renderpass_begin_info,
            VK_SUBPASS_CONTENTS_INLINE
        );

        return std::nullopt;
    }

    auto CommandBuffer::bindPipeline(PipelineBindPoint bind_point, const xgpu::Pipeline& pipeline) noexcept -> std::optional<Error>
    {
        auto vk_pipeline = pipeline.get_as<vk::Pipeline>();
        if (!vk_pipeline)
        {
            return Error("failed to get vk::Pipeline from xgpu::Pipeline");
        }

        vkCmdBindPipeline(_handle, getVulkanPipelineBindPoint(bind_point), vk_pipeline->handle());
        return std::nullopt;
    }

    auto CommandBuffer::setViewport(const ViewportDescription& info) noexcept -> std::optional<Error>
    {
        constexpr std::uint32_t first_viewport { 0 };
        constexpr std::uint32_t viewport_count { 1 };
        VkViewport viewport = getVulkanViewport(info);
        vkCmdSetViewport(_handle, first_viewport, viewport_count, &viewport);
        return std::nullopt;
    }

    auto CommandBuffer::setScissor(Rect2D scissor) noexcept -> std::optional<Error>
    {
        constexpr std::uint32_t first_scissor { 0 };
        constexpr std::uint32_t scissor_count { 1 };
        VkRect2D vk_scissor {
            .offset = {
                .x = scissor.x,
                .y = scissor.y,
            },
            .extent = {
                .width = scissor.extent.width,
                .height = scissor.extent.height,
            }
        };

        vkCmdSetScissor(_handle, first_scissor, scissor_count, &vk_scissor);
        return std::nullopt;
    }

    auto CommandBuffer::draw(std::uint32_t num_vertices) noexcept -> std::optional<Error>
    {
        constexpr std::uint32_t instance_count { 1 };
        constexpr std::uint32_t first_vertex { 0 };
        constexpr std::uint32_t first_instance { 0 };
        vkCmdDraw(_handle, num_vertices, instance_count, first_vertex, first_instance);
        return std::nullopt;
    }

    auto CommandBuffer::endRenderPass() noexcept -> std::optional<Error>
    {
        vkCmdEndRenderPass(_handle);
        return std::nullopt;
    }

    auto CommandBuffer::end() noexcept -> std::optional<Error>
    {
        const VkResult end_result = vkEndCommandBuffer(_handle);
        if (end_result != VK_SUCCESS)
        {
            return Error("vkEndCommandBuffer != VK_SUCCESS");
        }
        return std::nullopt;
    }

    auto CommandBuffer::memoryBarrier(const TextureBarrierDescription& description) noexcept -> std::optional<Error>
    {
        auto vk_image_view = description.texture.get_as<vk::ImageView>();
        if (!vk_image_view)
        {
            return Error("Failed to get vk::ImageView from xgpu::ImageView");
        }

        VkImageMemoryBarrier barrier {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
            .dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = vk_image_view->image(),
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = description.mip_level,
                .levelCount = 1,
                .baseArrayLayer = description.base_array_layer,
                .layerCount = 1,
            },
        };

        vkCmdPipelineBarrier(
            _handle, 
            convertPipelineStage(description.src_stage), 
            convertPipelineStage(description.dst_stage), 
            0, 
            0, 
            nullptr, 
            0, 
            nullptr, 
            1, 
            &barrier
        );

        return std::nullopt;
    }

    auto CommandBuffer::copyTextureToBuffer(
        const TextureCopyDescription& texture_copy, 
        xgpu::Buffer& dest
    ) noexcept -> std::optional<Error>
    {
        auto vk_image_view = texture_copy.image.get_as<vk::ImageView>();
        if (!vk_image_view)
        {
            return Error("failed to get vk::ImageView from xgpu::ImageView");
        }

        auto vk_buffer = dest.get_as<vk::Buffer>();

        VkBufferImageCopy region {
            .bufferOffset = 0,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = texture_copy.mip_level,
                .baseArrayLayer = texture_copy.base_array_layer,
                .layerCount = 1,
            },
            .imageOffset = { 0, 0, 0 },
            .imageExtent = {
                .width = texture_copy.extent.width,
                .height = texture_copy.extent.height,
                .depth = 1
            },
        };

        vkCmdCopyImageToBuffer(
            _handle, 
            vk_image_view->image(), 
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 
            vk_buffer->handle(), 
            1, 
            &region
        );

        return std::nullopt;
    }
} // namespace xgpu::vk
