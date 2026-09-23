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
#include "viewport.h"

namespace Renderer {
	namespace {
		void renderViewport(VulkanContext& ctx, VkCommandBuffer cmd, ViewportTarget& vp) {
			VulkanImage::transitionImage(cmd, vp.image, VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

			constexpr VkClearValue clear = VulkanCore::vec4ToClearValue(Config::editorBackgroundColor());

			VkRenderingAttachmentInfo color{};
			color.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			color.imageView   = vp.view;  // not swapchain
			color.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			color.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
			color.storeOp     = VK_ATTACHMENT_STORE_OP_STORE;
			color.clearValue  = clear;
			VkRenderingInfo info{};
			info.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
			info.renderArea.extent    = vp.extent;
			info.layerCount           = 1;
			info.colorAttachmentCount = 1;
			info.pColorAttachments    = &color;
			vkCmdBeginRendering(cmd, &info);
			vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, ctx.pipelines.filled);
			ctx.dynamicStates.viewport = {
				.x = 0.f, .y = 0.f,
				.width = static_cast<float>(vp.extent.width), .height = static_cast<float>(vp.extent.height),
				.minDepth = 0.f, .maxDepth = 1.f
			};
			ctx.dynamicStates.scissor = { {0, 0}, vp.extent };
			VulkanCommands::setupDynamicStates(ctx);
			vkCmdDraw(cmd, 3, 1, 0, 0);
			vkCmdEndRendering(cmd);
			// ImGui samples this in SHADER_READ_ONLY (must match AddTexture).
			VulkanImage::transitionImage(
				cmd, vp.image,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			);
		}

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
			ViewportTarget& vp = ctx.viewport;

			renderViewport(ctx, cmd, vp);

			VulkanImage::transitionImage(cmd,ctx.swapchainState.images[imageIndex],
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			);

			constexpr VkClearValue clear = VulkanCore::vec4ToClearValue(Config::editorBackgroundColor());

			VkRenderingAttachmentInfo color{};
			color.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			color.imageView   = ctx.swapchainState.views[imageIndex];
			color.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			color.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
			color.storeOp     = VK_ATTACHMENT_STORE_OP_STORE;
			color.clearValue  = clear;

			VkRenderingInfo info{};
			info.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
			info.renderArea.extent    = ctx.swapchainState.extent;
			info.layerCount           = 1;
			info.colorAttachmentCount = 1;
			info.pColorAttachments    = &color;

			vkCmdBeginRendering(cmd, &info);git
			UI::end(cmd);
			vkCmdEndRendering(cmd);

			VulkanImage::transitionImage(cmd, ctx.swapchainState.images[imageIndex],
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			);

			VK_CHECK(vkEndCommandBuffer(cmd), "Failed to end command buffer!");
		}

		bool beginDraw(VulkanContext& ctx, VkCommandBuffer cmd) {
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
				return false;
			}

			if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR) {
				throw std::runtime_error("Failed to acquire swapchain image!");
			}

			VK_CHECK(vkResetFences(ctx.device, 1, &VulkanCore::getCurrentFrame(ctx).renderFence), "Failed to reset Fence!");

			VK_CHECK(vkResetCommandBuffer(cmd, 0), "Failed to reset command buffer!");

			constexpr VkCommandBufferBeginInfo beginInfo = VulkanCommands::makeCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
			VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo), "Failed to begin command buffer!");
			return true;
		}
	}

	bool init(VulkanContext& ctx) {
		VulkanCore::init(ctx);
		VulkanCommands::init(ctx);
		VulkanSync::init(ctx);
		VulkanPipelines::init(ctx);

		return true	;
	}

	void prepareFrame(VulkanContext& ctx) {
		Viewport::ensureViewportSize(ctx, ctx.viewport);
	}

	void destroy(VulkanContext& ctx) {
		vkDeviceWaitIdle(ctx.device);

		VulkanPipelines::destroy(ctx);
		VulkanSync::destroy(ctx);
		VulkanCommands::destroy(ctx);
		VulkanCore::destroy(ctx);
	}

	void draw(VulkanContext& ctx) {
		VkCommandBuffer cmd = VulkanCore::getCurrentFrame(ctx).mainCommandBuffer;

		if (!beginDraw(ctx, cmd)) {
			return;
		}

		recordCommandBuffer(ctx, cmd);

		endDraw(ctx, cmd);
	}
}