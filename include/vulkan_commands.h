#pragma once

#include <vulkan/vulkan.h>

struct VulkanContext;

namespace VulkanCommands {
    void init(VulkanContext& ctx);
    void cleanup(VulkanContext& ctx);

    constexpr VkCommandBufferBeginInfo makeCommandBufferBeginInfo(VkCommandBufferUsageFlags flags) {
        VkCommandBufferBeginInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.pNext = nullptr;
        info.pInheritanceInfo = nullptr;
        info.flags = flags;
        return info;
    }

    constexpr VkCommandBufferSubmitInfo makeCommandBufferSubmitInfo(VkCommandBuffer cmd) {
        VkCommandBufferSubmitInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
        info.pNext = nullptr;
        info.commandBuffer = cmd;
        info.deviceMask = 0;

        return info;
    }
}