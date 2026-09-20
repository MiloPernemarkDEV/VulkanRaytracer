#include "pch.h"
#include "vulkan_sync.h"
#include "vulkan_core.h"

namespace VulkanSync {
    namespace {
        constexpr VkFenceCreateInfo makeFenceCreateInfo(VkFenceCreateFlags flags) {
            VkFenceCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            info.pNext = nullptr;
            info.flags = flags;
            return info;
        }

        constexpr VkSemaphoreCreateInfo makeSemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0) {
            VkSemaphoreCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            info.pNext = nullptr;
            info.flags = flags;
            return info;
        }
    } // unnamed namespace

    void init(VulkanContext& ctx) {
        const VkFenceCreateInfo fenceCreateInfo = makeFenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
        VkSemaphoreCreateInfo semaphoreCreateInfo = makeSemaphoreCreateInfo();

        for (int i = 0; i < Config::FRAME_OVERLAP; i++) {
            VK_CHECK(vkCreateFence(ctx.device, &fenceCreateInfo, nullptr, &ctx.frameStates[i].renderFence),
                "Failed to create fence!");

            VK_CHECK(vkCreateSemaphore(ctx.device, &semaphoreCreateInfo, nullptr, &ctx.frameStates[i].swapchainSemaphore),
                "Failed to create semaphore!");
            VK_CHECK(vkCreateSemaphore(ctx.device, &semaphoreCreateInfo, nullptr, &ctx.frameStates[i].renderSemaphore),
                "Failed to create semaphore!");
        }
    }

    void destroy(VulkanContext& ctx) {

    }

} // namespace VulkanSync