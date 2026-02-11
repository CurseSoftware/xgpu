#include "rhi/device.h"
#include "rhi/core.h"
#include <iostream>
#include <rhi/instance.h>
#include <rhi/vulkan.h>

auto main() -> int
{
    xgpu::InstanceContext instance_ctx = xgpu::vk::InstanceContext {
        .enable_debug = true
    };
    auto inst_exp = xgpu::Instance::create(instance_ctx);
    if (!inst_exp.has_value())
    {
        std::cerr << "Failed to create instance: " << inst_exp.unwrap_error().message << '\n';
        return 1;
    }

    auto inst = inst_exp.unwrap();

    xgpu::DefaultDeviceContext device_ctx { 
        .instance = inst,
        .graphics_preference = xgpu::Preference::Required,
        .transfer_preference = xgpu::Preference::Required,
        .compute_preference = xgpu::Preference::NoPreference
    };

    auto device_exp = xgpu::Device::create(device_ctx);
    if (!device_exp.has_value())
    {
        std::cerr << "Failed to create xgpu device: " << device_exp.unwrap_error().message << '\n';
        return 1;
    }

    auto device = device_exp.unwrap();

    device.destroy();
    inst.destroy();
    
    return 0;
}
