#include <xgpu/buffer.h>
#include <xgpu/command.h>
#include <xgpu/device.h>
#include <xgpu/core.h>
#include <xgpu/format.h>
#include <xgpu/framebuffer.h>
#include <xgpu/image_view.h>
#include <xgpu/pipeline.h>
#include <xgpu/pipeline_layout.h>
#include <xgpu/renderpass.h>
#include <xgpu/shader.h>
#include <xgpu/types.h>
#include <array>
#include <xgpu/instance.h>
#include <xgpu/vulkan.h>

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <vector>

// STB utils for image writing
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

auto readShader(const std::string& filepath) -> std::vector<char8_t>;

struct Vertex
{
    std::array<float, 3> position;

    static auto binding_description() -> xgpu::VertexInputDescription::Binding
    {
        return xgpu::VertexInputDescription::Binding {
            .binding = 0,
            .stride = sizeof(Vertex),
        };
    }

    static auto attribute_descriptions() -> std::array<xgpu::VertexInputDescription::AttributeDescription, 1>
    {
        return std::to_array<xgpu::VertexInputDescription::AttributeDescription>({
            {
                .binding = 0,
                .offset = offsetof(Vertex, position),
                .location = 0,
                .format = xgpu::Format::RGBA8_UINT,
            },
        });
    }
};

static constexpr auto g_vertices = std::to_array<Vertex>({
    { .position = { -0.5,  0.5, 0.0 } },
    { .position = {  0.5,  0.5, 0.0 } },
    { .position = {  0.0, -0.5, 0.0 } },
});

auto main() -> int
{
    xgpu::Extent2D image_extent {
        .width = 800,
        .height = 600,
    };

    // Information about image data
    std::uint32_t bytes_per_pixel { 4 };
    std::size_t bytes_per_row = image_extent.width * bytes_per_pixel;
    std::size_t bytes_per_image = bytes_per_row * image_extent.height;

    // Create the instance
    xgpu::InstanceContext instance_ctx = xgpu::vk::InstanceContext {
        .enable_debug = true
    };

    auto inst = xgpu::Instance::create(instance_ctx)
        .expect("Failed to create instance");

    auto device = xgpu::Device::create(xgpu::DefaultDeviceContext { 
        .instance = inst,
        .graphics_preference = xgpu::Preference::Required,
        .transfer_preference = xgpu::Preference::Required,
        .compute_preference = xgpu::Preference::NoPreference
    }).expect("Failed to create device");

    xgpu::OpenAttachmentDescription color_attachment {
        .format = xgpu::Format::RGBA8_UINT,
        .load_operation = xgpu::LoadOperation::Clear,
        .store_operation = xgpu::StoreOperation::Store,

        // Stencil defaults operations to `DontCare`
        // this is just here for example
        .stencil = {
            .load_operation = xgpu::LoadOperation::DontCare,
            .store_operation = xgpu::StoreOperation::DontCare
        },

        // .final_layout = xgpu::ImageLayout::Present
        .final_layout = xgpu::ImageLayout::ColorOptimal
    };

    auto image_view = xgpu::ImageView::create(device, xgpu::ImageViewDescription {
        .extent = {
            .width = image_extent.width,
            .height = image_extent.height,
            .depth = 1
        },
        .format = xgpu::Format::RGBA8_UINT,
        .mip_levels = 1,
        .array_layers = 1,
        .image_type = xgpu::ImageType::Type2D,
        .usage = xgpu::ImageUsage::ColorAttachment() | xgpu::ImageUsage::TransferDst() | xgpu::ImageUsage::TransferSrc(),
        .tiling = xgpu::ImageTiling::Optimal,
        .initial_layout = xgpu::ImageLayout::Undefined,
        .aspect = xgpu::ImageAspectFlags::Color(),
    }).expect("Failed to create image view");

    auto renderpass = xgpu::Renderpass::create(
        device, 
        {
            color_attachment
        },
        {
            xgpu::OpenSubpassDescription {
                .bind_point = xgpu::SubpassBindPoint::Graphics,
                // The 0 is the index reference to the color_attachment
                .attachments = { 
                    { .index = 0, .type = xgpu::AttachmentType::Color }
                }

            },
        }
    ).expect("Failed to create render pass");
    
    auto framebuffer = xgpu::Framebuffer::create(device, xgpu::FramebufferDescription {
        .renderpass = renderpass,
        .attachments = std::to_array<const std::reference_wrapper<xgpu::ImageView>>({
            image_view
        }),
        .width = image_extent.width,
        .height = image_extent.height,
    }).expect("Failed to create framebuffer");

    auto pipeline_layout = xgpu::PipelineLayout::create(device, xgpu::PipelineLayoutDescription {}).expect("Failed to create pipeline layout");

    auto vert_data = readShader("triangle.vert.spv");
    auto frag_data = readShader("triangle.frag.spv");

    auto vert_module = xgpu::ShaderModule::create(device, vert_data).expect("Failed to create vertex shader module");
    auto frag_module = xgpu::ShaderModule::create(device, frag_data).expect("Failed to create fragment shader module");

    auto render_pipeline = xgpu::Pipeline::create(device, xgpu::OpenGraphicsPipelineDescription {
        .renderpass = renderpass,
        .layout = pipeline_layout,
        .enable_depth_test = true,
        .subpass = 0,
        .viewports = std::to_array<xgpu::ViewportDescription>({
            {
                .width = static_cast<float>(image_extent.width),
                .height = static_cast<float>(image_extent.height),
                .x = 0.f,
                .y = 0.f,
                .min_depth = 0.0f,
                .max_depth = 0.0f,
            }
        }),
        .scissors = std::to_array<xgpu::ScissorDescription>({
            {
                .offset = {
                    .x = 0,
                    .y = 0,
                },
                .extent = {
                    .width = image_extent.width,
                    .height = image_extent.height
                }
            }
        }),
        
        .color_blend = {
            .attachments = std::array<xgpu::ColorBlendAttachmentStateDescription, 1>({
                xgpu::ColorBlendAttachmentStateDescription {
                    .color_blend_op = xgpu::BlendOperator::Add
                }
            })
        },
        .dynamic_state = {
            .states = std::to_array<xgpu::PipelineDynamicState>({
                xgpu::PipelineDynamicState::Viewport,
                xgpu::PipelineDynamicState::Scissor,
            })
        },
        .stages = {
            { xgpu::ShaderStageFlags::Vert, vert_module },
            { xgpu::ShaderStageFlags::Frag, frag_module },
        }
    }).expect("Failed to create render pipeline");

    auto graphics_pool = xgpu::CommandPool::create(device, xgpu::CommandPoolDescription 
        { .family_index = device.graphics_queue().value() 
    }).expect("Failed to create graphics command pool");

    auto command_buffers = graphics_pool.allocateBuffers(2).expect("Failed to allocate command buffers");

    // Record the commands
    {
        // auto command_buffers = std::move(command_buffers);
        auto& command_buffer = command_buffers[0];

        command_buffer->begin();
        command_buffer->beginRenderPass(xgpu::RenderPassBeginInfo {
            .renderpass = renderpass,
            .framebuffer = framebuffer,
            .render_area = {
                .offset = { .x = 0, .y = 0 },
                .extent = image_extent
            },
            .clear_color = xgpu::ClearColorValue{ .r = 100, .g = 100, .b = 255, .a = 255 }
        });

        command_buffer->bindPipeline(xgpu::PipelineBindPoint::Graphics, render_pipeline);
        command_buffer->setViewport(xgpu::ViewportDescription {
            .width = static_cast<float>(image_extent.width),
            .height = static_cast<float>(image_extent.height),
            .x = 0.0f,
            .y = 0.0f,
            .min_depth = 0.0f,
            .max_depth = 1.0f,
        });
        command_buffer->setScissor(xgpu::Rect2D {
            .x = 0,
            .y = 0,
            .extent = image_extent
        });
        command_buffer->draw(/* num_vertices */ 3);
        command_buffer->endRenderPass();
        
        auto storage_buffer = xgpu::Buffer::create(device, xgpu::BufferDescription {
            .size = bytes_per_image,
            .usage = xgpu::BufferUsageFlags::CopyDst() | xgpu::BufferUsageFlags::Storage(),
            .mapping = xgpu::BufferMapping::MapRead
        }).expect("Failed to create storage buffer");

        // Get the data from the framebuffer
        {
            command_buffer->memoryBarrier(xgpu::TextureBarrierDescription {
                .texture = image_view,
                .src_stage = xgpu::PipelineStage::Transfer,
                .dst_stage = xgpu::PipelineStage::Transfer,
                .mip_level = 0,
                .base_array_layer = 0,
                .extent = {
                    .width = image_extent.width,
                    .height = image_extent.height,
                    .depth = 1
                },
            });

            command_buffer->copyTextureToBuffer(
                xgpu::TextureCopyDescription {
                    .image = image_view,
                    .mip_level = 0,
                    .base_array_layer = 0,
                    .extent = {
                        .width = image_extent.width,
                        .height = image_extent.height,
                        .depth = 1
                    },
                }, 
                storage_buffer
            );

            command_buffer->end();
            device.submitSingle(xgpu::QueueFamilyIndex::Graphics, command_buffer.get());
            device.waitIdle();

            // Write the image data from the buffer to a PNG file
            {
                void* image_data { nullptr };
                storage_buffer.map(&image_data, bytes_per_image);
                stbi_write_png("triangle.png", image_extent.width, image_extent.height, 4, image_data, bytes_per_row);
                storage_buffer.unmap();
            }
        }
        
        storage_buffer.destroy();
    }

    graphics_pool.destroy();
    render_pipeline.destroy();
    frag_module.destroy();
    vert_module.destroy();
    pipeline_layout.destroy();
    framebuffer.destroy();
    renderpass.destroy();
    image_view.destroy();
    device.destroy();
    inst.destroy();
    
    return 0;
}

auto readShader(const std::string& filepath) -> std::vector<char8_t>
{
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    std::size_t filesize = file.tellg();
    std::vector<char8_t> data(filesize);
    file.seekg(0);
    file.read(reinterpret_cast<char*>(data.data()), filesize);
    file.close();

    return data;
}
