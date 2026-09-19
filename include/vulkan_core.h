#pragma once 

struct VulkanContext {
	VkInstance instance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
};

namespace VulkanCore {
	bool init(VulkanContext& ctx);
	void cleanup(VulkanContext& ctx);
}