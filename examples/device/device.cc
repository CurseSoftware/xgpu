#include "rhi/device.h"
#include "rhi/core.h"
#include <iostream>
#include <rhi/instance.h>
#include <rhi/vulkan.h>

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
        .compute_preference = rhi::Preference::None
    };

    auto device_exp = rhi::Device::create(device_ctx);
    if (!device_exp.has_value())
    {
        std::cerr << "Failed to create rhi device: " << device_exp.unwrap_error().message << '\n';
        return 1;
    }

    auto device = device_exp.unwrap();

    device.destroy();
    inst.destroy();
    
    return 0;
}
