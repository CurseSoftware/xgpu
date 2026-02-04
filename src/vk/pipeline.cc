#include "rhi/vk/core.h"
#include "core/log.h"
#include "rhi/vk/device.h"
#include "rhi/vk/pipeline.h"
#include "rhi/shader.h"
#include "rhi/vk/shader.h"

#include <functional>
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
        log::info("Pipeline viewports:          {}", viewports.size());
        log::info("Pipeline scissors:           {}", scissors.size());

        auto expected_shader_stage_infos = getVulkanShaderStageInfos(description.stages);
        if (!expected_shader_stage_infos.has_value())
        {
            return unexpected( expected_shader_stage_infos.unwrap_error() );
        }
        auto shader_stage_infos = expected_shader_stage_infos.unwrap();

        log::info("Pipeline shader stage infos: {}", shader_stage_infos.size());

        return ok(pipeline);
    }

    auto Pipeline::destroy() noexcept -> void
    {
    }
    
    auto getVulkanShaderStageInfos(const std::unordered_map<ShaderStageFlags, std::reference_wrapper<rhi::ShaderModule>>& shader_modules) -> expected<std::vector<VkPipelineShaderStageCreateInfo>, Error>
    {
        std::vector<VkPipelineShaderStageCreateInfo> infos {};
        for (const auto& [stage, module] : shader_modules)
        {
            auto vk_module = dynamic_cast<vk::ShaderModule*>(module.get().handle());
            if (!vk_module)
            {
                return unexpected( Error("Failed to get vulkan shader module from rhi::ShaderModule") );
            }

            infos.emplace_back( VkPipelineShaderStageCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = getVulkanShaderStageFlags(stage),
                .module = vk_module->handle(),
                .pName = "main",
            });
        }

        return ok(infos);
    }
    
    auto getVulkanShaderStageInfo(ShaderStageFlags stage, VkShaderModule module) -> VkPipelineShaderStageCreateInfo
    {
        return VkPipelineShaderStageCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = getVulkanShaderStageFlags(stage),
            .module = module,
            .pName = "main"
        };
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
