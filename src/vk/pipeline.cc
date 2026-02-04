#include "rhi/vk/device.h"
#include "rhi/vk/pipeline.h"
#include "rhi/vk/core.h"
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    auto Pipeline::from_open(rhi::Device& p_device, const GraphicsPipelineDescription& p_description) noexcept -> expected<vk::Pipeline, Error>
    {
        Pipeline pipeline {};
        auto* vk_device = dynamic_cast<vk::Device*>(p_device.handle());
        if (!vk_device)
        {
            return unexpected( Error("Failed to get vk::Device from input rhi::Device object") );
        }

        // There is only one description for now 
        auto description = std::get<OpenGraphicsPipelineDescription>(p_description);

        auto viewports = getVulkanViewports(description.viewports);
        auto scissors = getVulkanScissors(description.scissors);

        auto expected_shader_stages = getVulkanShaderStages(vk_device->handle(), description.stages);
        if (!expected_shader_stages.has_value())
        {
            return unexpected( expected_shader_stages.unwrap_error() );
        }

        return ok(pipeline);
    }

    auto createVulkanShaderModule(VkDevice device, const ShaderModule& module) -> expected<VkShaderModule, Error>
    {
        VkShaderModule shader { VK_NULL_HANDLE };
        VkShaderModuleCreateInfo create_info {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = module.size(),
            .pCode = reinterpret_cast<const std::uint32_t*>(module.data()),
        };

        const VkResult create_result = vkCreateShaderModule(device, &create_info, nullptr, &shader);
        if (create_result != VK_SUCCESS)
        {
            return unexpected( Error("Failed to create vulkan shader module") );
        }

        return ok(shader);
    }
    
    auto getVulkanShaderStageFlags(ShaderStageFlags stage) -> VkShaderStageFlagBits
    {
        switch (stage)
        {
            case ShaderStageFlags::Vert:
                return VK_SHADER_STAGE_VERTEX_BIT;
            case ShaderStageFlags::Frag:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
        }

        return VK_SHADER_STAGE_ALL;
    }

    auto getVulkanShaderStage(VkDevice device, ShaderStageFlags stage, const ShaderModule& module) -> expected<VkPipelineShaderStageCreateInfo, Error>
    {
        auto expected_shader = createVulkanShaderModule(device, module);
        if (!expected_shader.has_value())
        {
            return unexpected( expected_shader.unwrap_error() );
        }

        auto shader = expected_shader.unwrap();
        
        return ok(VkPipelineShaderStageCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = getVulkanShaderStageFlags(stage),
            .module = shader,
            .pName = "main"
        });
    }

    auto getVulkanShaderStages(VkDevice device, const std::unordered_map<ShaderStageFlags, ShaderModule>& stages) -> expected<std::vector<VkPipelineShaderStageCreateInfo>, Error>
    {
        std::vector<VkPipelineShaderStageCreateInfo> create_infos {};

        for (const auto& [stage, module] : stages)
        {
            auto expected_info = getVulkanShaderStage(device, stage, module);
            if (!expected_info.has_value())
            {
                return unexpected( expected_info.unwrap_error() );
            }
            create_infos.push_back(expected_info.unwrap());
        }

        return ok(create_infos);
    }
    
    auto getVulkanScissor(const ScissorDescription& scissor) -> VkRect2D
    {
        return VkRect2D {
            .offset = {
                .x = scissor.offset.x,
                .y = scissor.offset.y,
            },
            .extent = {
                .width = scissor.extent.width,
                .height = scissor.extent.height,
            },
        };
    }

    auto getVulkanScissors(std::span<ScissorDescription> scissors) -> std::vector<VkRect2D>
    {
        std::vector<VkRect2D> vk_scissors {};

        for (const auto& scissor : scissors)
        {
            vk_scissors.push_back(getVulkanScissor(scissor));
        }

        return vk_scissors;
    }

    auto getVulkanViewports(std::span<ViewportDescription> viewports) -> std::vector<VkViewport>
    {
        std::vector<VkViewport> vk_viewports {};

        for (const auto& viewport : viewports)
        {
            vk_viewports.push_back(getVulkanViewport(viewport));
        }

        return vk_viewports;
    }

    auto getVulkanViewport(const ViewportDescription& viewport) -> VkViewport
    {
        return VkViewport {
            .x = viewport.x,
            .y = viewport.y,
            .width = viewport.width,
            .height = viewport.height,
            .minDepth = viewport.min_depth,
            .maxDepth = viewport.max_depth,
        };
    }
} // namespace rhi::vk
