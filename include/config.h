#pragma once

#include "defines.h"
#include <array>
#include <vulkan/vulkan.h>

namespace Config
{
	constexpr auto GLOBAL_ARENA_SIZE = 100_MB;

	/* Window configuration */
	constexpr s32 WINDOW_WIDTH = 1280;
	constexpr s32 WINDOW_HEIGHT = 720;
	constexpr auto WINDOW_TITLE = "Raytracer";

	/* Paths and folders */
	constexpr auto ASSETS_FOLDER_NAME = "assets";

	/* Renderer configuration */
	constexpr std::array<const char*, 3> REQUIRED_VK_EXTENSIONS = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};
	constexpr std::array<const char*, 1> REQUIRED_VK_LAYERS = {
		"VK_LAYER_KHRONOS_validation"
	};
#ifdef NDEBUG
	constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
	constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif
	constexpr auto REQUIRED_VULKAN_VERSION = VK_API_VERSION_1_3;
	constexpr auto APPLICATION_VERSION = VK_MAKE_VERSION(1, 0, 0);
}