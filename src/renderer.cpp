#include "pch.h"
#include "renderer.h"
#include "vulkan_core.h"
#include "vulkan_commands.h"

namespace Renderer {

	bool init(VulkanContext& ctx) {
		VulkanCore::init(ctx);
		VulkanCommands::init(ctx);



		return true	;
	}

	void terminate(VulkanContext& ctx) {
		vkDeviceWaitIdle(ctx.device);

		VulkanCommands::cleanup(ctx);
		VulkanCore::cleanup(ctx);	
	}
}