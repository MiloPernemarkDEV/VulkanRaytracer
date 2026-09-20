#pragma once

struct VulkanContext;

namespace VulkanSync {
    void init(VulkanContext& context);
    void destroy(VulkanContext& context);
}