#include "pipeline.h"
#include "rhi/vk/core.h"
#include "core/log.h"
#include "rhi/vk/device.h"
#include "rhi/vk/pipeline.h"
#include "rhi/shader.h"
#include "rhi/vk/shader.h"
#include "types.h"
#include "vk/format.h"
#include "vk/pipeline_layout.h"
#include "vk/renderpass.h"
#include "vk/vk_utils.h"

#include <cstdint>
#include <functional>
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    auto Pipeline::from_open(xgpu::Device& p_device, const GraphicsPipelineDescription& p_description) noexcept -> expected<vk::Pipeline, Error>
    {
        auto* vk_device = dynamic_cast<vk::Device*>(p_device.handle());
        if (!vk_device)
        {
            return unexpected( Error("Failed to get vk::Device from input rhi::Device object") );
        }
        Pipeline pipeline { vk_device->handle() };

        // There is only one description for now 
        auto description = std::get<OpenGraphicsPipelineDescription>(p_description);

        auto* vk_renderpass = dynamic_cast<vk::Renderpass*>(description.renderpass.handle());

        auto viewports = getVulkanViewports(description.viewports);
        auto scissors = getVulkanScissors(description.scissors);
        auto viewport_state = getVulkanViewportState(viewports, scissors);

        auto expected_shader_stage_infos = getVulkanShaderStageInfos(description.stages);
        if (!expected_shader_stage_infos.has_value())
        {
            return unexpected( expected_shader_stage_infos.unwrap_error() );
        }
        auto shader_stage_infos = expected_shader_stage_infos.unwrap();

        auto vertex_attribute_descriptions = getVulkanVertexInputAttributeDescriptions(description.vertex_input.attribute_descriptions);
        auto vertex_binding_descriptions = getVulkanVertexInputBindingDescriptions(description.vertex_input.bindings);
        auto vertex_input_state_info = getVulkanVertexInputState(description.vertex_input, vertex_attribute_descriptions, vertex_binding_descriptions);

        auto tesselation_state_info = getVulkanTesselationState(description.tesselation);

        auto color_blend_attachment_states = getVulkanColorBlendAttachments(description.color_blend.attachments);
        auto color_blend_state = getVulkanColorBlendState(description.color_blend, color_blend_attachment_states);

        auto multisample_state = getVulkanMultisampleState(description.multisample);

        auto rasterization_state = getVulkanRasterizationState(description.rasterization);

        auto dynamic_states = getVulkanDynamicStates(description.dynamic_state.states);
        auto dynamic_state = getVulkanDynamicState(dynamic_states);

        auto depth_stencil_state = getVulkanDepthStencil(description.depth_stencil);

        auto input_assembly_state = getVulkanInputAssemblyState(description.input_assembly);

        auto expected_layout = getVulkanPipelineLayout(description.layout);
        if (!expected_layout.has_value())
        {
            return unexpected( expected_layout.unwrap_error() );
        }
        auto layout = expected_layout.unwrap();

        log::info("Pipeline viewports:          {}", viewports.size());
        log::info("Pipeline scissors:           {}", scissors.size());
        log::info("Pipeline shader stage infos: {}", shader_stage_infos.size());

        VkGraphicsPipelineCreateInfo pipeline_info {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = static_cast<std::uint32_t>(shader_stage_infos.size()),
            .pStages = shader_stage_infos.data(),
            .pVertexInputState = &vertex_input_state_info,
            .pInputAssemblyState = &input_assembly_state,
            .pTessellationState = &tesselation_state_info,
            .pViewportState = &viewport_state,
            .pRasterizationState = &rasterization_state,
            .pMultisampleState = &multisample_state,
            .pDepthStencilState = &depth_stencil_state,
            .pColorBlendState = &color_blend_state,
            .pDynamicState = &dynamic_state,
            .layout = layout,
            .renderPass = vk_renderpass->handle(),
            .subpass = description.subpass,
        };

        VkPipelineCache cache { VK_NULL_HANDLE };
        const VkResult create_result = vkCreateGraphicsPipelines(
            vk_device->handle(), 
            cache, 
            1, 
            &pipeline_info, 
            nullptr, 
            &pipeline._handle
        );
        if (create_result != VK_SUCCESS)
        {
            return unexpected( Error("vkCreateGraphicsPipeline != VK_SUCCESS") );
        }

        return ok(pipeline);
    }

    auto Pipeline::destroy() noexcept -> void
    {
        log::trace("Destroying vulkan pipeline...");
        vkDestroyPipeline(_device, _handle, nullptr);
        log::trace("Destroyed.");
    }

    auto getVulkanPipelineLayout(const xgpu::PipelineLayout& layout) noexcept -> expected<VkPipelineLayout, Error>
    {
        auto* expected_layout = dynamic_cast<vk::PipelineLayout*>(layout.handle());
        if (!expected_layout)
        {
            return unexpected( Error("Unable to get vk::PipelineLayout from rhi::PipelineLayout") );
        }

        return ok(expected_layout->handle());
    }
    
    auto getVulkanShaderStageInfos(const std::unordered_map<ShaderStageFlags, std::reference_wrapper<xgpu::ShaderModule>>& shader_modules) -> expected<std::vector<VkPipelineShaderStageCreateInfo>, Error>
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

    auto getVulkanScissors(std::span<const ScissorDescription> scissors) -> std::vector<VkRect2D>
    {
        std::vector<VkRect2D> vk_scissors {};

        for (const auto& scissor : scissors)
        {
            vk_scissors.push_back(getVulkanScissor(scissor));
        }

        return vk_scissors;
    }

    auto getVulkanViewports(std::span<const ViewportDescription> viewports) -> std::vector<VkViewport>
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
    
    auto getVulkanVertexInputAttributeDescriptions(std::span<VertexInputDescription::AttributeDescription> attributes) -> std::vector<VkVertexInputAttributeDescription>
    {
        std::vector<VkVertexInputAttributeDescription> descriptions {};

        for (const auto& attribute : attributes)
        {
            descriptions.emplace_back( VkVertexInputAttributeDescription {
                .location = attribute.location,
                .binding = attribute.binding,
                .format = convertFormat(attribute.format),
                .offset = attribute.offset,
            });
        }

        return descriptions;
    }

    auto getVulkanVertexInputBindingDescriptions(std::span<VertexInputDescription::Binding> bindings) -> std::vector<VkVertexInputBindingDescription>
    {
        std::vector<VkVertexInputBindingDescription> descriptions {};

        for (const auto& binding : bindings)
        {
            descriptions.emplace_back( VkVertexInputBindingDescription {
                .binding = binding.binding,
                .stride = binding.stride,

                // TODO: update this to allow instancing?
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            });
        }

        return descriptions;
    }

    // Get the vulkan vertex input info from the rhi description
    auto getVulkanVertexInputState(
        const VertexInputDescription& desc,
        std::span<VkVertexInputAttributeDescription> attribute_descriptions,
        std::span<VkVertexInputBindingDescription> binding_descriptions
        ) -> VkPipelineVertexInputStateCreateInfo
    {
        return VkPipelineVertexInputStateCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount = static_cast<std::uint32_t>(binding_descriptions.size()),
            .pVertexBindingDescriptions = binding_descriptions.data(),
            .vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attribute_descriptions.size()),
            .pVertexAttributeDescriptions = attribute_descriptions.data(),
        };
    }
    
    auto getVulkanTesselationState(const TesselationStateDescription& desc) -> VkPipelineTessellationStateCreateInfo
    {
        return VkPipelineTessellationStateCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
            .patchControlPoints = desc.patch_control_points,
        };
    }
    
    auto getVulkanViewportState(
        std::span<VkViewport> viewports,
        std::span<VkRect2D> scissors
        ) -> VkPipelineViewportStateCreateInfo
    {
        return VkPipelineViewportStateCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = static_cast<std::uint32_t>(viewports.size()),
            .pViewports = viewports.data(),
            .scissorCount = static_cast<std::uint32_t>(scissors.size()),
            .pScissors = scissors.data()
        };
    }
    
    auto getVulkanColorBlendAttachments(std::span<const ColorBlendAttachmentStateDescription> attachments) -> std::vector<VkPipelineColorBlendAttachmentState>
    {
        std::vector<VkPipelineColorBlendAttachmentState> states {};
        
        for (const auto& attachment : attachments)
        {
            states.emplace_back( VkPipelineColorBlendAttachmentState {
                .blendEnable = attachment.blend_enable,
                .srcColorBlendFactor = convertBlendFactor(attachment.src_color_blend_factor),
                .dstColorBlendFactor = convertBlendFactor(attachment.dst_color_blend_factor),
                .colorBlendOp = convertBlendOperator(attachment.color_blend_op),
                .srcAlphaBlendFactor = convertBlendFactor(attachment.src_alpha_blend_factor),
                .dstAlphaBlendFactor = convertBlendFactor(attachment.dst_alpha_blend_factor),
                .alphaBlendOp = convertBlendOperator(attachment.alpha_blend_op),
                .colorWriteMask = convertColorComponentFlags(attachment.color_write_mask),
            });
        }

        return states;
    }
    
    auto getVulkanColorBlendState(const ColorBlendStateDescription& desc, std::span<VkPipelineColorBlendAttachmentState> attachments) -> VkPipelineColorBlendStateCreateInfo
    {
        return VkPipelineColorBlendStateCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = desc.logic_op_enable,
            .logicOp = convertLogicOperator(desc.logic_op),
            .attachmentCount = static_cast<std::uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .blendConstants = { 
                desc.blend_constants[0],
                desc.blend_constants[1],
                desc.blend_constants[2],
                desc.blend_constants[3],
            }
        };
    }
    
    auto getVulkanMultisampleState(const MultisampleStateDescription& desc) -> VkPipelineMultisampleStateCreateInfo
    {
        return VkPipelineMultisampleStateCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = convertSampleCount(desc.rasterization_samples),
            .sampleShadingEnable = desc.shading_enable ? VK_TRUE : VK_FALSE,
            .minSampleShading = desc.min_sample_shading,
            .pSampleMask = desc.sample_mask,
            .alphaToCoverageEnable = desc.alpha_to_coverage,
            .alphaToOneEnable = desc.alpha_to_one ? VK_TRUE : VK_FALSE,
        };
    }
    
    auto getVulkanRasterizationState(const RasterizationStateDescription& desc) -> VkPipelineRasterizationStateCreateInfo
    {
        return VkPipelineRasterizationStateCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = desc.depth_clamp ? VK_TRUE : VK_FALSE,
            .rasterizerDiscardEnable = desc.rasterizer_discard ? VK_TRUE : VK_FALSE,
            .polygonMode = convertPolygonMode(desc.polygon_mode),
            .cullMode = convertCullMode(desc.cull_mode),
            .frontFace = convertFrontFace(desc.front_face),
            .depthBiasEnable = desc.depth_bias ? VK_TRUE : VK_FALSE,
            .depthBiasConstantFactor = desc.depth_bias_constant_factor,
            .depthBiasClamp = desc.depth_bias_clamp,
            .depthBiasSlopeFactor = desc.depth_bias_slope_factor,
            .lineWidth = desc.line_width,
        };
    }
    
    auto getVulkanDynamicStates(std::span<const PipelineDynamicState> states) -> std::vector<VkDynamicState>
    {
        std::vector<VkDynamicState> vk_states {};

        for (const auto& state : states)
        {
            vk_states.push_back(convertDynamicState(state));
        }

        return vk_states;
    }
    
    auto getVulkanDynamicState(std::span<VkDynamicState> dynamic_states) -> VkPipelineDynamicStateCreateInfo
    {
        return VkPipelineDynamicStateCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount = static_cast<std::uint32_t>(dynamic_states.size()),
            .pDynamicStates = dynamic_states.data(),
        };
    }
    
    auto getVulkanDepthStencil(const DepthStencilStateDescription& desc) -> VkPipelineDepthStencilStateCreateInfo
    {
        return VkPipelineDepthStencilStateCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable = desc.depth_test              ? VK_TRUE : VK_FALSE,
            .depthWriteEnable = desc.depth_write            ? VK_TRUE : VK_FALSE,
            .depthCompareOp = convertCompareOperator(desc.depth_compare_op),
            .depthBoundsTestEnable = desc.depth_bounds_test ? VK_TRUE : VK_FALSE,
            .stencilTestEnable = desc.stencil_test          ? VK_TRUE : VK_FALSE,
            .front {
                .failOp = convertStencilOperator(desc.front.fail_op),
                .passOp = convertStencilOperator(desc.front.pass_op),
                .depthFailOp = convertStencilOperator(desc.front.depth_fail_op),
                .compareOp = convertCompareOperator(desc.front.compare_op),
                .compareMask = desc.front.compare_mask,
                .writeMask = desc.front.write_mask,
                .reference = desc.front.reference,
            },
            .back = {
                .failOp = convertStencilOperator(desc.back.fail_op),
                .passOp = convertStencilOperator(desc.back.pass_op),
                .depthFailOp = convertStencilOperator(desc.back.depth_fail_op),
                .compareOp = convertCompareOperator(desc.back.compare_op),
                .compareMask = desc.back.compare_mask,
                .writeMask = desc.back.write_mask,
                .reference = desc.back.reference,
            },
            .minDepthBounds = desc.min_depth_bounds,
            .maxDepthBounds = desc.max_depth_bounds,
        };
    }
    
    auto getVulkanInputAssemblyState(const InputAssemblyStateDescription& desc) -> VkPipelineInputAssemblyStateCreateInfo
    {
        return VkPipelineInputAssemblyStateCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = convertTopology(desc.topology),
            .primitiveRestartEnable = desc.primitive_restart ? VK_TRUE : VK_FALSE,
        };
    }
} // namespace xgpu::vk
