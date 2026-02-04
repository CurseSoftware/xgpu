#include "rhi/device.h"
#include "rhi/core.h"
#include "rhi/renderpass.h"
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <rhi/instance.h>
#include <rhi/vulkan.h>
#include <stdexcept>
#include <string_view>
#include <vector>

auto readShader(const std::string& filepath) -> std::vector<char8_t>;

auto main() -> int
{
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
        .format = rhi::Format::RG8_UINT,
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

    auto vert_data = readShader("triangle.vert.spv");
    auto frag_data = readShader("triangle.frag.spv");

    std::cout << "Vert: " << vert_data.size() << " bytes read\n";
    std::cout << "Frag: " << frag_data.size() << " bytes read\n";

    renderpass.destroy();
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


