#pragma once

#include "defines.h"

struct SwapchainState {
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	std::vector<VkImage> images;
	std::vector<VkImageView> views;
};

struct VulkanContext {
	VkInstance instance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	u32 queueFamilyIndex = UINT32_MAX;
	VkSurfaceKHR surface = VK_NULL_HANDLE;

	SwapchainState swapchainState;
};

namespace VulkanCore {
	bool init(VulkanContext& ctx);
	void cleanup(VulkanContext& ctx);
}