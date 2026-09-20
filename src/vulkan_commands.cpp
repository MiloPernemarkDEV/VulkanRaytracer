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
    } // unnamed namespace

    void init(VulkanContext& ctx) {
        const VkCommandPoolCreateInfo commandPoolInfo = makeCommandPoolInfo(ctx.queueFamilyIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

        for (size_t i = 0; i < Config::FRAME_OVERLAP; i++) {
            if (vkCreateCommandPool(ctx.device, &commandPoolInfo, nullptr,
                &ctx.frameStates[i].commandPool) != VK_SUCCESS) {
                throw std::runtime_error("failed to create the command pool!");
            }

            VkCommandBufferAllocateInfo cmdAllocInfo{};
            cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            cmdAllocInfo.pNext = nullptr;
            cmdAllocInfo.commandPool = ctx.frameStates[i].commandPool;
            cmdAllocInfo.commandBufferCount = 1;
            cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            if (vkAllocateCommandBuffers(ctx.device, &cmdAllocInfo, &ctx.frameStates[i].mainCommandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to allocate command buffer!");
            }
        }
    }

    void cleanup(VulkanContext& ctx) {
        for (size_t i = 0; i < Config::FRAME_OVERLAP; i++) {
            vkDestroyCommandPool(ctx.device, ctx.frameStates[i].commandPool, nullptr);
        }
    }
} // namespace VulkanCommands