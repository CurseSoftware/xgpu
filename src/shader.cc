#include "rhi/shader.h"
#include "expected.h"
#include "vk/shader.h"
#include <memory>

namespace rhi
{
    auto ShaderModule::create(rhi::Device& device, std::span<char8_t> data) noexcept -> expected<ShaderModule, Error>
    {
        ShaderModule module {};
        switch (device.backend())
        {
            case Backend::Vulkan:
#ifdef RHI_COMPILE_VULKAN_BACKEND
                {
                    auto expected_shader = vk::ShaderModule::from_open(device, data);
                    if (!expected_shader.has_value())
                    {
                        return unexpected( expected_shader.unwrap_error() );
                    }

                    module._handle = std::make_unique<vk::ShaderModule>(expected_shader.unwrap());
                    return ok(module);
                }
#else
                return unexpected( Error("Failed to create vulkan shader module: Vulkan backend not compiled") );
#endif // RHI_COMPILE_VULKAN_BACKEND
            break;
            default:
                return unexpected( Error("Shader module creation on backend not supported") );
        }
        
        return unexpected( Error("Shader module creation on backend not supported") );
    }
} // namespace rhi
