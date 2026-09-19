#pragma once

#include "defines.h"

#ifdef _WIN32
#include <windows.h>
#include "vulkan_core.h"
#include "defines.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Window {
    bool create(int nCmdShow);
    s32 getWindowHeight();
    s32 getWindowWidth();
    void* getWindowHandle();
    void pollEvents();
    std::vector<const char*> getVkExtensions();
    bool close();
    void createSurface(VulkanContext& ctx);
    void destroySurface(VulkanContext& ctx);

    VkExtent2D getExtent2D(const VkSurfaceCapabilitiesKHR& capabilities, HWND hwnd);
    HWND getHandle();
}

#else 

#endif

