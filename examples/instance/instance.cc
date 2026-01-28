#include <iostream>
#include <rhi/instance.h>
#include <rhi/vulkan.h>

auto main() -> int
{
    rhi::InstanceContext instance_ctx = rhi::vk::InstanceContext {
        .enable_debug = true
    };
    auto inst = rhi::Instance::create(instance_ctx);
    if (!inst.has_value())
    {
        std::cerr << "Failed to create instance: " << inst.unwrap_error().message << '\n';
        return 1;
    }

    inst->destroy();
    
    return 0;
}
