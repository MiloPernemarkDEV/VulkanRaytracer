#pragma once

struct VulkanContext;

namespace VulkanCommands {
    void init(VulkanContext& ctx);
    void cleanup(VulkanContext& ctx);
}