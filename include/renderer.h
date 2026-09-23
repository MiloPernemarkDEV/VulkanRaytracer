#pragma once 

struct VulkanContext;

#include <vk_mem_alloc.h>

namespace Renderer {
	bool init(VulkanContext& ctx); 
	void destroy(VulkanContext& ctx);
	void prepeareFrame(VulkanContext& ctx);
	void draw(VulkanContext& ctx);
}