#include "pch.h"
#include "renderer.h"
#include "vulkan_core.h"
#include "vulkan_commands.h"
#include "vulkan_sync.h"

namespace Renderer {

	bool init(VulkanContext& ctx) {
		VulkanCore::init(ctx);
		VulkanCommands::init(ctx);
		VulkanSync::init(ctx);


		return true	;
	}

	void terminate(VulkanContext& ctx) {
		vkDeviceWaitIdle(ctx.device);

		VulkanSync::destroy(ctx);
		VulkanCommands::cleanup(ctx);
		VulkanCore::cleanup(ctx);	
	}

	void draw(VulkanContext& ctx) {
		VK_CHECK(vkWaitForFences(ctx.device, 1, &VulkanCore::getCurrentFrame(ctx).renderFence, true, secondsToNano(1)),
			"Failed to wait for fences");

		VK_CHECK(vkResetFences(ctx.device, 1, &VulkanCore::getCurrentFrame(ctx).renderFence), "Failed to reset Fence!");

		u32 swapchainImageIndex{};

		VK_CHECK(vkAcquireNextImageKHR(
			ctx.device,
			ctx.swapchainState.swapchain,
			secondsToNano(1),
			VulkanCore::getCurrentFrame(ctx).swapchainSemaphore,
			nullptr, &swapchainImageIndex),
			"Failed to acquire swapchain image!"
		);

		VkCommandBuffer cmd = VulkanCore::getCurrentFrame(ctx).mainCommandBuffer;
		VK_CHECK(vkResetCommandBuffer(cmd, 0), "Failed to reset command buffer!");

		VkCommandBufferBeginInfo beginInfo = VulkanCommands::makeCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo), "Failed to begin command buffer!");
	}
}