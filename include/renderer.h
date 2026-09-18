#pragma once 

#include <vulkan/vulkan.h>

struct VulkanContext {
	VkInstance instance = VK_NULL_HANDLE;
};

namespace Renderer {
	bool init(VulkanContext& ctx); 
}