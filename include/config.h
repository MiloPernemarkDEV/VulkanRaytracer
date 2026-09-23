#pragma once

#include "defines.h"
#include <array>
#include <vulkan/vulkan.h>
#include "vec4.h"
#include <imgui.h>

namespace Config
{
    // =========================================================================
    // Application & Engine Info
    // =========================================================================
    constexpr auto globalArenaSize = 100_MB;
    constexpr auto appName         = "Raytracer";
    constexpr auto engineName      = "INeedAjobEngine";

    // =========================================================================
    // Window Configuration
    // =========================================================================
    constexpr s32  windowWidth  = 1280;
    constexpr s32  windowHeight = 720;
    constexpr auto windowTitle  = "Raytracer";

    // =========================================================================
    // File Paths & Directories
    // =========================================================================
    constexpr auto assetFolderName = "assets";
    #define FILLED_PIPELINE_VERT_PATH "shaders/spirv/triangle.vs.spv"
    #define FILLED_PIPELINE_FRAG_PATH "shaders/spirv/triangle.fs.spv"
    constexpr auto uiMainFontPath = "assets/fonts/Roboto-Regular.ttf";

    // =========================================================================
    // Vulkan & Renderer Settings
    // =========================================================================
    constexpr auto requiredVulkanVersion = VK_API_VERSION_1_3;
    constexpr auto applicationVersion    = VK_MAKE_VERSION(1, 0, 0);
    constexpr u32  frameOverlap          = 2;
    constexpr auto shaderEntryName       = "main";
    constexpr float standardLineWidth   = 1.0f;

#ifdef NDEBUG
    constexpr bool enableValidationLayers = false;
#else
    constexpr bool enableValidationLayers = true;
#endif

    // =========================================================================
    // Extensions, Layers & Dynamic State
    // =========================================================================
    constexpr std::array<const char*, 3> requiredInstanceExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };

    constexpr std::array<const char*, 4> requiredDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME
    };

    constexpr std::array<const char*, 1> requiredVulkanLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    // Pipeline state that can be mutated at draw time without pipeline recreation
    constexpr std::array dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_CULL_MODE,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };
    
    // =========================================================================
    // Editor styling
    // =========================================================================
    constexpr Math::vec4 editorBackgroundColor() {
        return {0.015f, 0.02f, 0.04f, 1.0f};
    }

    constexpr ImVec4 windowBackgroundColorImGui() {
        constexpr auto c = editorBackgroundColor();
        return {c.x, c.y, c.z, c.w};
    }
}
