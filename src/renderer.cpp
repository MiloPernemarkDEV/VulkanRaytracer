#include "pch.h"
#include "renderer.h"
#include <vulkan/vulkan.h>
#include "vulkan_core.h"
#include "vulkan_commands.h"
#include "vulkan_sync.h"
#include "vulkan_image.h"
#include "vulkan_pipelines.h"
#include <imgui_impl_vulkan.h>
#include "ui.h"

namespace Renderer {
	namespace {
		void presentImage(VulkanContext& ctx) {
			VkPresentInfoKHR presentInfo{};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.pNext = nullptr;
			presentInfo.pSwapchains = &ctx.swapchainState.swapchain;
			presentInfo.swapchainCount = 1;
			presentInfo.pWaitSemaphores = &VulkanCore::getCurrentFrame(ctx).renderSemaphore;
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pImageIndices = &ctx.swapchainImageIndex;

			const VkResult presentResult = vkQueuePresentKHR(ctx.graphicsQueue, &presentInfo);
			if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR) {
				VulkanCore::recreateSwapchain(ctx);
			} else if (presentResult != VK_SUCCESS) {
				throw std::runtime_error("Failed to present queue!");
			}
		}

		void endDraw(VulkanContext& ctx, VkCommandBuffer cmd) {
			VkCommandBufferSubmitInfo cmdInfo = VulkanCommands::makeCommandBufferSubmitInfo(cmd);
			VkSemaphoreSubmitInfo waitInfo = VulkanSync::makeSemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
				VulkanCore::getCurrentFrame(ctx).swapchainSemaphore);
			VkSemaphoreSubmitInfo signalInfo = VulkanSync::makeSemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, VulkanCore::getCurrentFrame(ctx).renderSemaphore);

			const VkSubmitInfo2 submit = VulkanSync::makeSubmitInfo(&cmdInfo, &signalInfo, &waitInfo);
			VK_CHECK(vkQueueSubmit2(ctx.graphicsQueue, 1, &submit, VulkanCore::getCurrentFrame(ctx).renderFence), "Failed to submit command buffer!");

			presentImage(ctx);
			ctx.currentFrame++;
		}

		void recordCommandBuffer(VulkanContext& ctx, VkCommandBuffer cmd) {
			const u32 imageIndex = ctx.swapchainImageIndex;

			VulkanImage::transitionImage(
				cmd,
				ctx.swapchainState.images[imageIndex],
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			);

			const VkClearValue clearValue{
				.color = {{0.015f, 0.02f, 0.04f, 1.0f}}
			};

			VkRenderingAttachmentInfo colorAttachment{};
			colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			colorAttachment.imageView = ctx.swapchainState.views[imageIndex];
			colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.clearValue = clearValue;

			VkRenderingInfo renderingInfo{};
			renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
			renderingInfo.renderArea.offset = {0, 0};
			renderingInfo.renderArea.extent =  ctx.swapchainState.extent;
			renderingInfo.layerCount = 1;
			renderingInfo.colorAttachmentCount = 1;
			renderingInfo.pColorAttachments = &colorAttachment;

			vkCmdBeginRendering(cmd, &renderingInfo);
			vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, ctx.pipelines.filled);
			VulkanCommands::setupDynamicStates(ctx);

			vkCmdDraw(cmd, 3, 1, 0, 0);

			UI::end(cmd);
			vkCmdEndRendering(cmd);

			VulkanImage::transitionImage(
				cmd,
				ctx.swapchainState.images[imageIndex],
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			);

			VK_CHECK(vkEndCommandBuffer(cmd),
				"Failed to end command buffer!"
			);
		}

		void beginDraw(VulkanContext& ctx, VkCommandBuffer cmd) {
			VK_CHECK(vkWaitForFences(ctx.device, 1, &VulkanCore::getCurrentFrame(ctx).renderFence, true, secondsToNano(1)),
			"Failed to wait for fences");

			const VkResult acquireResult = vkAcquireNextImageKHR(
				ctx.device,
				ctx.swapchainState.swapchain,
				secondsToNano(1),
				VulkanCore::getCurrentFrame(ctx).swapchainSemaphore,
				nullptr, &ctx.swapchainImageIndex);

			if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR) {
				VulkanCore::recreateSwapchain(ctx);
				return;
			}

			if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR) {
				throw std::runtime_error("Failed to acquire swapchain image!");
			}

			VK_CHECK(vkResetFences(ctx.device, 1, &VulkanCore::getCurrentFrame(ctx).renderFence), "Failed to reset Fence!");

			VK_CHECK(vkResetCommandBuffer(cmd, 0), "Failed to reset command buffer!");

			constexpr VkCommandBufferBeginInfo beginInfo = VulkanCommands::makeCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
			VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo), "Failed to begin command buffer!");
		}
	}

	bool init(VulkanContext& ctx) {
		VulkanCore::init(ctx);
		VulkanCommands::init(ctx);
		VulkanSync::init(ctx);
		VulkanPipelines::init(ctx);


		return true	;
	}

	void terminate(VulkanContext& ctx) {
		vkDeviceWaitIdle(ctx.device);

		VulkanPipelines::destroy(ctx);
		VulkanSync::destroy(ctx);
		VulkanCommands::destroy(ctx);
		VulkanCore::destroy(ctx);
	}

	void draw(VulkanContext& ctx) {
		VkCommandBuffer cmd = VulkanCore::getCurrentFrame(ctx).mainCommandBuffer;

		beginDraw(ctx, cmd);

		recordCommandBuffer(ctx, cmd);

		endDraw(ctx, cmd);
	}
}