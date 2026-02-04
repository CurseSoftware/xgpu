#include "rhi/shader.h"

namespace rhi
{
    auto ShaderModule::create(rhi::Device& p_device, const std::vector<char8_t>& p_data) noexcept -> ShaderModule
    {
        ShaderModule shader_module{ p_device, p_data };
        return shader_module;
    }
} // namespace rhi
