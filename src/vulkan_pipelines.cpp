#include "pch.h"
#include "vulkan_pipelines.h"
#include "shader.h"
#include "vulkan_core.h"
#include "config.h"
#include <array>
#include "defines.h"

namespace VulkanPipelines {
    namespace {
        void createPipelineLayout(VulkanContext& ctx) {
            VkPipelineLayoutCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            info.pNext = nullptr;
            info.setLayoutCount = 0;
            info.pushConstantRangeCount = 0;

            VK_CHECK(vkCreatePipelineLayout(
                ctx.device,
                &info,
                nullptr,
                &ctx.pipelines.layout),
                "Failed to create pipeline layout!"
            );
        }

        VkPipeline makePipeline(VulkanContext &ctx, const Descriptor &desc) {
            const auto vertCode = Shader::readFile(desc.vertexShader);
            const auto fragCode = Shader::readFile(desc.fragmentShader);

            VkShaderModule vertModule = Shader::createModule(ctx.device, vertCode);
            VkShaderModule fragModule = Shader::createModule(ctx.device, fragCode);

            VkPipelineShaderStageCreateInfo vertStageInfo{};
            vertStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vertStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            vertStageInfo.module = vertModule;
            vertStageInfo.pName = Config::shaderEntryName;

            VkPipelineShaderStageCreateInfo fragStageInfo{};
            fragStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            fragStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            fragStageInfo.module = fragModule;
            fragStageInfo.pName = Config::shaderEntryName;

            std::array shaderStages  =
            {
                vertStageInfo,
                fragStageInfo
            };

            VkPipelineDynamicStateCreateInfo dynamicState{};
            dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            dynamicState.dynamicStateCount = static_cast<u32>(Config::dynamicStates.size());
            dynamicState.pDynamicStates = Config::dynamicStates.data();

            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

            VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
            inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssembly.pNext = nullptr;
            inputAssembly.topology = desc.topology;
            inputAssembly.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(ctx.swapchainState.extent.width);
            viewport.height = static_cast<float>(ctx.swapchainState.extent.height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor{};
            scissor.offset = { 0, 0 };
            scissor.extent = ctx.swapchainState.extent;

            VkPipelineViewportStateCreateInfo viewportState{};
            viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportState.viewportCount = 1;
            viewportState.scissorCount = 1;

            VkPipelineRasterizationStateCreateInfo rasterizer{};
            rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizer.pNext = nullptr;
            rasterizer.depthClampEnable = VK_FALSE;
            rasterizer.rasterizerDiscardEnable = VK_FALSE;
            rasterizer.polygonMode = desc.polygonMode;
            rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
            rasterizer.frontFace = desc.frontFace;
            rasterizer.depthBiasEnable = VK_FALSE;
            rasterizer.lineWidth = 1.0f;

            VkPipelineMultisampleStateCreateInfo multisample{};
            multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisample.pNext = nullptr;
            multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            multisample.sampleShadingEnable = VK_FALSE;

            VkPipelineColorBlendAttachmentState colorBlendAttachment{};
            colorBlendAttachment.blendEnable = desc.blending ? VK_TRUE : VK_FALSE;
            colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
            colorBlendAttachment.colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT |
                VK_COLOR_COMPONENT_G_BIT |
                VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT;

            VkPipelineColorBlendStateCreateInfo colorBlending{};
            colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            colorBlending.logicOpEnable = VK_FALSE;
            colorBlending.logicOp = VK_LOGIC_OP_COPY;
            colorBlending.attachmentCount = 1;
            colorBlending.pAttachments = &colorBlendAttachment;

            VkPipelineRenderingCreateInfo renderingInfo{};
            renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
            renderingInfo.colorAttachmentCount = 1;
            renderingInfo.pColorAttachmentFormats = &ctx.swapchainState.imageFormat;

            VkPipelineDepthStencilStateCreateInfo depthStencil{};
            depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depthStencil.depthTestEnable = desc.depthTest ? VK_TRUE : VK_FALSE;
            depthStencil.depthWriteEnable = desc.depthWrite ? VK_TRUE : VK_FALSE;

            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.pNext = &renderingInfo;
            pipelineInfo.stageCount = static_cast<u32>(shaderStages.size());
            pipelineInfo.pStages = shaderStages.data();
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = &inputAssembly;
            pipelineInfo.pViewportState = &viewportState;
            pipelineInfo.pRasterizationState = &rasterizer;
            pipelineInfo.pMultisampleState = &multisample;
            pipelineInfo.pColorBlendState = &colorBlending;
            pipelineInfo.pDynamicState = &dynamicState;
            pipelineInfo.layout = ctx.pipelines.layout;
            pipelineInfo.renderPass = VK_NULL_HANDLE;

            VkPipeline newPipeline = VK_NULL_HANDLE;
            VK_CHECK(vkCreateGraphicsPipelines(ctx.device, nullptr, 1, &pipelineInfo, nullptr, &newPipeline),
                "Failed to create pipeline!"
            );

            vkDestroyShaderModule(ctx.device, vertModule, nullptr);
            vkDestroyShaderModule(ctx.device, fragModule, nullptr);

            return newPipeline;
        }

        void createFilledPipeline(VulkanContext& ctx) {
            Descriptor desc{};
            desc.vertexShader = "shaders/spirv/triangle.vs.spv";
            desc.fragmentShader = "shaders/spirv/triangle.fs.spv";
            desc.polygonMode = VK_POLYGON_MODE_FILL;
            desc.frontFace = VK_FRONT_FACE_CLOCKWISE;
            desc.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

            desc.depthTest = false;
            desc.depthWrite = false;
            desc.blending = false;

            ctx.pipelines.filled = makePipeline(ctx, desc);
        }

        void createWireframePipeline(VulkanContext& ctx) {

        }
    } // Unnamed namespace


    void init(VulkanContext& ctx) {
        createPipelineLayout(ctx);
        createFilledPipeline(ctx);
    }

    void destroy(VulkanContext& ctx) {
        vkDestroyPipeline(ctx.device, ctx.pipelines.filled, nullptr);
        vkDestroyPipelineLayout(ctx.device, ctx.pipelines.layout, nullptr);
    }

} // namespace VulkanPipelines