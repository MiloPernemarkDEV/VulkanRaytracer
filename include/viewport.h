#pragma once

struct VulkanContext;
struct ViewportTarget;

#include <vulkan/vulkan.h>

namespace Viewport {
    void destroyViewportTarget(VulkanContext& ctx, ViewportTarget& vp);
    void createViewportTarget(VulkanContext& ctx, ViewportTarget& vp, uint32_t w, uint32_t h);
    void ensureViewportSize(VulkanContext& ctx, ViewportTarget& vp);
}