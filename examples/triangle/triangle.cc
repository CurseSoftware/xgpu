#include "rhi/command.h"
#include "rhi/core/log.h"
#include "rhi/device.h"
#include "rhi/core.h"
#include "rhi/format.h"
#include "rhi/framebuffer.h"
#include "rhi/image_view.h"
#include "rhi/pipeline.h"
#include "rhi/pipeline_layout.h"
#include "rhi/renderpass.h"
#include "rhi/shader.h"
#include "rhi/types.h"
#include <array>
#include <cstddef>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <rhi/instance.h>
#include <rhi/vulkan.h>
#include <stdexcept>
#include <string_view>
#include <vector>

auto readShader(const std::string& filepath) -> std::vector<char8_t>;

struct Vertex
{
    std::array<float, 3> position;

    static auto binding_description() -> rhi::VertexInputDescription::Binding
    {
        return rhi::VertexInputDescription::Binding {
            .binding = 0,
            .stride = sizeof(Vertex),
        };
    }

    static auto attribute_descriptions() -> std::array<rhi::VertexInputDescription::AttributeDescription, 1>
    {
        return std::to_array<rhi::VertexInputDescription::AttributeDescription>({
            {
                .binding = 0,
                .offset = offsetof(Vertex, position),
                .location = 0,
                .format = rhi::Format::RGB32_FLOAT,
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
    rhi::Extent2D image_extent {
        .width = 600,
        .height = 800,
    };

    rhi::InstanceContext instance_ctx = rhi::vk::InstanceContext {
        .enable_debug = true
    };
    auto inst_exp = rhi::Instance::create(instance_ctx);
    if (!inst_exp.has_value())
    {
        std::cerr << "Failed to create instance: " << inst_exp.unwrap_error().message << '\n';
        return 1;
    }

    auto inst = inst_exp.unwrap();

    rhi::DefaultDeviceContext device_ctx { 
        .instance = inst,
        .graphics_preference = rhi::Preference::Required,
        .transfer_preference = rhi::Preference::Required,
        .compute_preference = rhi::Preference::NoPreference
    };

    auto device_exp = rhi::Device::create(device_ctx);
    if (!device_exp.has_value())
    {
        std::cerr << "Failed to create rhi device: " << device_exp.unwrap_error().message << '\n';
        return 1;
    }

    auto device = device_exp.unwrap();

    rhi::OpenAttachmentDescription color_attachment {
        .format = rhi::Format::RGB32_FLOAT,
        .load_operation = rhi::LoadOperation::Clear,
        .store_operation = rhi::StoreOperation::Store,

        // Stencil defaults operations to `DontCare`
        // this is just here for example
        .stencil = {
            .load_operation = rhi::LoadOperation::DontCare,
            .store_operation = rhi::StoreOperation::DontCare
        },

        .final_layout = rhi::ImageLayout::Present
    };

    rhi::ImageViewDescription image_view_desc {
        .extent = {
            .width = image_extent.width,
            .height = image_extent.height,
            .depth = 1
        },
        .format = rhi::Format::RGB32_FLOAT,
        .mip_levels = 1,
        .array_layers = 1,
        .image_type = rhi::ImageType::Type2D,
        .usage = rhi::ImageUsage::ColorAttachment(),
        .tiling = rhi::ImageTiling::Optimal,
        .initial_layout = rhi::ImageLayout::Undefined,
        .aspect = rhi::ImageAspectFlags::Color(),
    };
    auto expected_image_view = rhi::ImageView::create(device, image_view_desc);
    if (!expected_image_view.has_value())
    {
        std::cerr << "Failed to create image view: " << expected_image_view.unwrap_error().message << '\n';
        return 1;
    }
    auto image_view = expected_image_view.unwrap();

    auto renderpass_exp = rhi::Renderpass::create(
        device, 
        {
            color_attachment
        },
        {
            rhi::OpenSubpassDescription {
                .bind_point = rhi::SubpassBindPoint::Graphics,
                // The 0 is the index reference to the color_attachment
                .attachments = { 
                    { .index = 0, .type = rhi::AttachmentType::Color }
                }
            }
        }
    );
    if (!renderpass_exp.has_value())
    {
        std::cerr << "Failed to create renderpass: " << renderpass_exp.unwrap_error().message << '\n';
        return 1;
    }
    auto renderpass = renderpass_exp.unwrap();

    rhi::FramebufferDescription framebuffer_desc {
        .renderpass = renderpass,
        .attachments = std::to_array<const std::reference_wrapper<rhi::ImageView>>({
            image_view
        }),
        .width = image_extent.width,
        .height = image_extent.height,
    };
    auto expected_framebuffer = rhi::Framebuffer::create(device, framebuffer_desc);
    if (!expected_framebuffer.has_value())
    {
        rhi::log::error("HERE3");
        std::cerr << "Failed to create framebuffer: " << expected_framebuffer.unwrap_error().message << '\n';
        return 1;
    }
    auto framebuffer = expected_framebuffer.unwrap();

    rhi::PipelineLayoutDescription layout_description {};
    auto expected_layout = rhi::PipelineLayout::create(device, layout_description);
    if (!expected_layout.has_value())
    {
        std::cerr << "Failed to create pipeline layout: " << expected_layout.unwrap_error().message << '\n';
        return 1;
    }
    auto layout = expected_layout.unwrap();

    auto vert_data = readShader("triangle.vert.spv");
    auto frag_data = readShader("triangle.frag.spv");

    auto expected_vert_module = rhi::ShaderModule::create(device, vert_data);
    auto expected_frag_module = rhi::ShaderModule::create(device, frag_data);

    if (!expected_vert_module.has_value())
    {
        std::cerr << "Failed to create vertex shader module: " << expected_vert_module.unwrap_error().message << '\n';
    }
    if (!expected_frag_module.has_value())
    {
        std::cerr << "Failed to create fragment shader module: " << expected_vert_module.unwrap_error().message << '\n';
    }

    auto vert_module = expected_vert_module.unwrap();
    auto frag_module = expected_frag_module.unwrap();

    rhi::OpenGraphicsPipelineDescription pipeline_description {
        .renderpass = renderpass,
        .layout = layout,
        .enable_depth_test = true,
        .subpass = 0,
        .viewports = std::to_array<rhi::ViewportDescription>({
            {
                .width = static_cast<float>(image_extent.width),
                .height = static_cast<float>(image_extent.height),
                .x = 0.f,
                .y = 0.f,
                .min_depth = 0.0f,
                .max_depth = 0.0f,
            }
        }),
        .scissors = std::to_array<rhi::ScissorDescription>({
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
            .attachments = std::array<rhi::ColorBlendAttachmentStateDescription, 1>()
        },
        .dynamic_state = {
            .states = std::to_array<rhi::PipelineDynamicState>({
                rhi::PipelineDynamicState::Viewport,
                rhi::PipelineDynamicState::Scissor,
            })
        },
        .stages = {
            { rhi::ShaderStageFlags::Vert, vert_module },
            { rhi::ShaderStageFlags::Frag, frag_module },
        }
    };

    auto expected_pipeline = rhi::Pipeline::create(device, pipeline_description);
    if (!expected_pipeline.has_value())
    {
        std::cerr << "Failed to create pipeline: " << expected_pipeline.unwrap_error().message << '\n';
        return 1;
    }
    auto pipeline = expected_pipeline.unwrap();

    std::cout << "Pipeline created successfully.\n";
    std::cout << "Vert: " << vert_data.size() << " bytes read\n";
    std::cout << "Frag: " << frag_data.size() << " bytes read\n";

    auto expected_graphics_pool = rhi::CommandPool::create(device, { .family_index = device.graphics_queue().value() });
    if (!expected_graphics_pool.has_value())
    {
        std::cerr << "Failed to create graphics pool: " << expected_graphics_pool.unwrap_error().message << '\n';
        return 1;
    }
    auto graphics_pool = expected_graphics_pool.unwrap();

    auto expected_buffers = graphics_pool.allocateBuffers(2);
    if (!expected_buffers.has_value())
    {
        std::cerr << "Failed to allocate command buffers: " << expected_buffers.unwrap_error().message << '\n';
        return 1;
    }
    std::cout << "Command buffers allocated.\n";

    graphics_pool.destroy();
    pipeline.destroy();
    frag_module.destroy();
    vert_module.destroy();
    layout.destroy();
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


