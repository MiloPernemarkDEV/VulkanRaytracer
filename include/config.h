#pragma once

#include "defines.h"
#include <array>
#include <vulkan/vulkan.h>

namespace Config
{
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

	constexpr bool ENABLE_VALIDATION_LAYERS = true;
}
