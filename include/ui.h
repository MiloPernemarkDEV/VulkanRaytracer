#pragma once

struct VulkanContext;

namespace UI {
#ifdef _WIN32
    void init(HWND hwnd, VulkanContext& ctx);
    void begin();
    void draw();
    void end(VkCommandBuffer commandBuffer);
    void shutdown(VulkanContext& ctx);
#else


#endif
}