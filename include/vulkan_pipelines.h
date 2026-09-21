#pragma once

struct VulkanContext;

namespace VulkanPipelines {
    void init(VulkanContext& ctx);
    void destroy(VulkanContext& ctx);
    void setupDynamicStates(VulkanContext& ctx);
}