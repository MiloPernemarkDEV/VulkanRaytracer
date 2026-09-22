#pragma once

struct VulkanContext;
struct ViewportTarget;

namespace UI {
#ifdef _WIN32
    void init(HWND hwnd, VulkanContext& ctx);
    void begin();
    void draw(ViewportTarget &target);
    void end(VkCommandBuffer commandBuffer);
    void destroy(VulkanContext& ctx);
#else


#endif
}