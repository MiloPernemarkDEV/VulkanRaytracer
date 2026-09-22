#include "pch.h"
#include "viewport.h"
#include <imgui_impl_vulkan.h>
#include "vulkan_core.h"
#include "defines.h"

namespace Viewport {
    void destroyViewportTarget(VulkanContext &ctx, ViewportTarget &vp) {
        if (vp.descSet) {
            ImGui_ImplVulkan_RemoveTexture(vp.descSet);
            vp.descSet = VK_NULL_HANDLE;
        }
        if (vp.view) {
            vkDestroyImageView(ctx.device, vp.view, nullptr);
            vp.view = VK_NULL_HANDLE;
        }

        if (vp.image) {
            vkDestroyImage(ctx.device, vp.image, nullptr);
            vp.image = VK_NULL_HANDLE;
        }

        if (vp.memory) {
            vkFreeMemory(ctx.device, vp.memory, nullptr);
            vp.memory = VK_NULL_HANDLE;
        }

        if (vp.sampler) {
            vkDestroySampler(ctx.device, vp.sampler, nullptr);
            vp.sampler = VK_NULL_HANDLE;
        }
    }

    void createViewportTarget(VulkanContext &ctx, ViewportTarget &vp, uint32_t w, uint32_t h) {
        destroyViewportTarget(ctx, vp);
        w = std::max(1u, w);
        h = std::max(1u, h);
        vp.extent = {.width = w, .height = h};

        VkImageCreateInfo imageInfo{};
        imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType     = VK_IMAGE_TYPE_2D;
        imageInfo.format        = vp.format;
        imageInfo.extent        = { w, h, 1 };
        imageInfo.mipLevels     = 1;
        imageInfo.arrayLayers   = 1;
        imageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage         = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VK_CHECK(vkCreateImage(ctx.device, &imageInfo, nullptr, &vp.image), "viewport image");

        VkMemoryRequirements memReq{};
        vkGetImageMemoryRequirements(ctx.device, vp.image, &memReq);

        VkMemoryAllocateInfo alloc{};
        alloc.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc.allocationSize  = memReq.size;
        alloc.memoryTypeIndex = VulkanCore::findMemoryType(
            ctx.physicalDevice,
            memReq.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );
        VK_CHECK(vkAllocateMemory(ctx.device, &alloc, nullptr, &vp.memory), "viewport mem");
        vkBindImageMemory(ctx.device, vp.image, vp.memory, 0);

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image    = vp.image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format   = vp.format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.layerCount = 1;
        VK_CHECK(vkCreateImageView(ctx.device, &viewInfo, nullptr, &vp.view), "viewport view");

        if (!vp.sampler) {
            VkSamplerCreateInfo samp{};
            samp.sType        = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samp.magFilter    = VK_FILTER_LINEAR;
            samp.minFilter    = VK_FILTER_LINEAR;
            samp.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            samp.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            samp.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            VK_CHECK(vkCreateSampler(ctx.device, &samp, nullptr, &vp.sampler), "viewport sampler");
        }

        vp.descSet = ImGui_ImplVulkan_AddTexture(
        vp.sampler,
        vp.view,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );
    }

    void ensureViewportSize(VulkanContext& ctx, ViewportTarget& vp) {
        if (vp.image && vp.extent.width == vp.desiredW && vp.extent.height == vp.desiredH)
            return;
        vkDeviceWaitIdle(ctx.device);
        createViewportTarget(ctx, vp, vp.desiredW, vp.desiredH);
    }
}
