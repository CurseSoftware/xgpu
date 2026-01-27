#include <iostream>
#include <rhi/instance.h>
#include <rhi/vulkan.h>

auto main() -> int
{
    rhi::InstanceContext instance_ctx = rhi::vk::InstanceContext {};
    auto inst = rhi::Instance::create(instance_ctx);
    if (!inst.has_value())
    {
        std::cerr << "Failed to create instance\n";
        return 1;
    }
    
    return 0;
}
