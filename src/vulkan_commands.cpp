#include "pch.h"
#include "vulkan_core.h"
#include "vulkan_commands.h"

namespace VulkanCommands {
    namespace {
        constexpr VkCommandPoolCreateInfo makeCommandPoolInfo(u32 queueFamilyIndex, VkCommandPoolCreateFlags flags) {
            VkCommandPoolCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            info.pNext = nullptr;
            info.queueFamilyIndex = queueFamilyIndex;
            info.flags = flags;
            return info;
        }

        constexpr VkCommandBufferAllocateInfo makeCmdBufferAllocInfo(VkCommandPool pool) {
            VkCommandBufferAllocateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            info.pNext = nullptr;
            info.commandPool = pool;
            info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            info.commandBufferCount = 1;
            return info;
        }
    } // unnamed namespace

    void init(VulkanContext &ctx) {
        const VkCommandPoolCreateInfo commandPoolInfo = makeCommandPoolInfo(
            ctx.queueFamilyIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

        for (size_t i = 0; i < Config::frameOverlap; i++) {
            VK_CHECK(vkCreateCommandPool(ctx.device, &commandPoolInfo, nullptr,&ctx.frameStates[i].commandPool),
                "failed to create command pool!"
            );

            const VkCommandBufferAllocateInfo cmdAllocInfo = makeCmdBufferAllocInfo(ctx.frameStates[i].commandPool);

            VK_CHECK(vkAllocateCommandBuffers(ctx.device, &cmdAllocInfo, &ctx.frameStates[i].mainCommandBuffer),
                "failed to allocate command buffer!"
            );
        }
    }

    void cleanup(VulkanContext &ctx) {
        for (size_t i = 0; i < Config::frameOverlap; i++) {
            vkDestroyCommandPool(ctx.device, ctx.frameStates[i].commandPool, nullptr);
        }
    }
} // namespace VulkanCommands
