#include "pch.h"
#include "renderer.h"
#include "config.h"
#include "vulkan_core.h"
#include "defines.h"

namespace Renderer {

	bool init(VulkanContext& ctx)
	{
		if (!VulkanCore::init(ctx)) {
			return false;
		}

		return true	;
	}

	void terminate(VulkanContext& ctx) {
		VulkanCore::cleanup(ctx);	
	}

}