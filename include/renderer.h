#pragma once 

#include <vulkan/vulkan.h>
#include "vulkan_core.h"

namespace Renderer {
	bool init(VulkanContext& ctx); 
	void terminate(VulkanContext& ctx);
	void draw(VulkanContext& ctx);
}