#pragma once

#include <vulkan/vulkan.h>

namespace VulkanImage {
    void transitionImage(VkCommandBuffer cmd, VkImage image, VkImageLayout current, VkImageLayout newLayout);

    constexpr VkImageSubresourceRange makeImageSubresourceRange(VkImageAspectFlags aspectMask) {
        VkImageSubresourceRange subImage {};
        subImage.aspectMask = aspectMask;
        subImage.baseMipLevel = 0;
        subImage.levelCount = VK_REMAINING_MIP_LEVELS;
        subImage.baseArrayLayer = 0;
        subImage.layerCount = VK_REMAINING_ARRAY_LAYERS;

        return subImage;
    }
}