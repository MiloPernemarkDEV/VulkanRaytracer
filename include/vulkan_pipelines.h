#pragma once

#include <vulkan/vulkan.h>

struct VulkanContext;


namespace VulkanPipelines {
    enum class PipelineType {
        // use with material registry
    };

    struct Descriptor {
        const char*         vertexShader   = nullptr;
        const char*         fragmentShader = nullptr;

        VkPolygonMode       polygonMode    = VK_POLYGON_MODE_FILL;
        VkFrontFace         frontFace      = VK_FRONT_FACE_CLOCKWISE;
        VkPrimitiveTopology topology       = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        bool                depthTest      = false;
        bool                depthWrite     = false;
        bool                blending       = false;
    };

    void init(VulkanContext& ctx);
    void destroy(VulkanContext& ctx);
}