#include "pch.h"

#include "window.h"
#include "windowsx.h"
#include "config.h"

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
		switch (msg)
		{
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
			CW_USEDEFAULT, CW_USEDEFAULT, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT,
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
}
