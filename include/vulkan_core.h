#pragma once

#include "defines.h"
#include "vulkan_commands.h"
#include "config.h"

struct FrameState {
	VkCommandPool commandPool;
	VkCommandBuffer mainCommandBuffer;
	VkSemaphore swapchainSemaphore;
	VkSemaphore renderSemaphore;
	VkFence renderFence;

};

struct SwapchainState {
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	std::vector<VkImage> images;
	std::vector<VkImageView> views;
	VkFormat imageFormat = VK_FORMAT_UNDEFINED;
};

struct VulkanContext {
	VkInstance instance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;

	u32 queueFamilyIndex = UINT32_MAX;
	VkQueue graphicsQueue = VK_NULL_HANDLE;

	VkSurfaceKHR surface = VK_NULL_HANDLE;

	SwapchainState swapchainState;
	u32 currentFrame = 0;
	std::array<FrameState, Config::FRAME_OVERLAP> frameStates;
};

namespace VulkanCore {
	void init(VulkanContext& ctx);
	void cleanup(VulkanContext& ctx);
	void recreateSwapchain(VulkanContext& ctx);

	[[nodiscard]] FrameState& getCurrentFrame(VulkanContext& ctx);
}