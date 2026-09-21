#pragma once

#include "defines.h"
#include "vulkan_commands.h"
#include "config.h"

struct DynamicStates {
	VkViewport viewport{};
	VkRect2D scissor{};
};

struct Pipelines {
	VkPipelineLayout layout = VK_NULL_HANDLE;
	VkPipeline filled = VK_NULL_HANDLE;
	VkPipeline wireframe = VK_NULL_HANDLE;
	VkPipeline vertex = VK_NULL_HANDLE;
};

struct FrameState {
	VkCommandPool commandPool = VK_NULL_HANDLE;
	VkCommandBuffer mainCommandBuffer = VK_NULL_HANDLE;
	VkSemaphore swapchainSemaphore = VK_NULL_HANDLE;
	VkSemaphore renderSemaphore = VK_NULL_HANDLE;
	VkFence renderFence = VK_NULL_HANDLE;
};

struct SwapchainState {
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	std::vector<VkImage> images;
	std::vector<VkImageView> views;
	VkExtent2D extent{};
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
	SwapchainState swapchainState{};
	u32 swapchainImageIndex{};
	u32 currentFrame = 0;
	std::array<FrameState, Config::frameOverlap> frameStates{};
	Pipelines pipelines{};
	DynamicStates dynamicStates{};
};

namespace VulkanCore {
	void init(VulkanContext& ctx);
	void destroy(VulkanContext& ctx);
	void recreateSwapchain(VulkanContext& ctx);

	[[nodiscard]] FrameState& getCurrentFrame(VulkanContext& ctx);
}