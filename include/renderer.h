#pragma once 

struct VulkanContext;

namespace Renderer {
	bool init(VulkanContext& ctx); 
	void terminate(VulkanContext& ctx);
	void draw(VulkanContext& ctx);
}