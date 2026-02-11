#include "rhi/vk/shader.h"
#include "core/log.h"
#include "rhi/vk/device.h"

#include "rhi/vk/core.h"

namespace xgpu::vk
{
    auto ShaderModule::from_open(xgpu::Device& device, std::span<char8_t> data) -> expected<vk::ShaderModule, Error>
    {
        if (device.backend() != Backend::Vulkan)
        {
            return unexpected( Error("Cannot create vulkan shader module from non-vulkan device") );
        }

        auto vk_device = reinterpret_cast<vk::Device*>(device.handle());
        if (!vk_device)
        {
            return unexpected( Error("Failed to get vulkan device from rhi::Device") );
        }

        ShaderModule shader { vk_device->handle(), data };

        VkShaderModuleCreateInfo create_info {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = data.size(),
            .pCode = reinterpret_cast<const std::uint32_t*>(data.data()),
        };

        const VkResult create_result = vkCreateShaderModule(vk_device->handle(), &create_info, nullptr, &shader._handle);
        if (create_result != VK_SUCCESS)
        {
            return unexpected( Error("vkCreateShaderModule != VK_SUCCESS") );
        }
        
        return ok(shader);
    }

    auto ShaderModule::destroy() noexcept -> void
    {
        log::trace("Destroying vulkan shader module...");
        vkDestroyShaderModule(_device, _handle, nullptr);
        log::trace("Destroyed.");
    }
} // namespace xgpu::vk
