#pragma once 

struct VulkanContext;

namespace Renderer {
	bool init(VulkanContext& ctx); 
	void destroy(VulkanContext& ctx);
	void prepareFrame(VulkanContext& ctx);
	void draw(VulkanContext& ctx);
}