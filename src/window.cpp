#include "pch.h"

#include "window.h"
#include "windowsx.h"
#include "config.h"
#include "math_util.h"
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static HWND g_hWnd;
static HINSTANCE g_hInstance;
static bool g_CloseWindow = false;

namespace Window 
{	
	namespace
	{
		void OnDestroy(HWND hwnd) {
			g_CloseWindow = true;
			PostQuitMessage(0);
		}
	}

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
			return true;
		}

		switch (msg) {
			HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);


		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		return 0;
	}

	bool create(int nCmdShow) {
		g_hInstance = GetModuleHandle(nullptr);

		const wchar_t CLASS_NAME[] = L"Raytracer";

		WNDCLASSW wc{};
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = g_hInstance;
		wc.lpszClassName = CLASS_NAME;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		RegisterClassW(&wc);

		g_hWnd = CreateWindowEx(0, CLASS_NAME, L"raytracer", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, Config::windowWidth, Config::windowHeight,
			nullptr, nullptr, g_hInstance, nullptr);

		ShowWindow(g_hWnd, nCmdShow);

		return true;
	}

	s32 getWindowHeight()
	{
		return s32();
	}

	s32 getWindowWidth()
	{
		return s32();
	}

	void* getWindowHandle()
	{
		return nullptr;
	}

	void pollEvents() {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	bool close()
	{
		return g_CloseWindow;
	}

	bool isMinimized()
	{
		if (g_hWnd == nullptr || IsIconic(g_hWnd)) {
			return true;
		}

		RECT rect{};
		if (!GetClientRect(g_hWnd, &rect)) {
			return true;
		}

		return (rect.right - rect.left) == 0 || (rect.bottom - rect.top) == 0;
	}

	void createSurface(VulkanContext& ctx) {
		VkSurfaceKHR surface = VK_NULL_HANDLE;

		VkWin32SurfaceCreateInfoKHR info{};
		info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		info.pNext = nullptr;
		info.flags = 0;
		info.hinstance = g_hInstance;
		info.hwnd = g_hWnd;

		if (vkCreateWin32SurfaceKHR(ctx.instance, &info, nullptr, &ctx.surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface!");
		}
	}

	void destroySurface(VulkanContext& ctx) {
		vkDestroySurfaceKHR(ctx.instance, ctx.surface, nullptr);
	}

	VkExtent2D getExtent2D(const VkSurfaceCapabilitiesKHR& capabilities,HWND hwnd) {
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}

		RECT rect{};
		if (!GetClientRect(hwnd, &rect)) {
			return capabilities.currentExtent;
		}

		const u32 width = static_cast<u32>(rect.right - rect.left);

		const u32 height = static_cast<u32>(rect.bottom - rect.top);

		if (width == 0 || height == 0) {
			return { 0, 0 };
		}

		VkExtent2D extent{ width, height };

		extent.width = Math::clamp(extent.width,
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width);

		extent.height = Math::clamp(extent.height,
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height);

		return extent;
	}

	HWND getHandle() {
		return g_hWnd;
	}
}
