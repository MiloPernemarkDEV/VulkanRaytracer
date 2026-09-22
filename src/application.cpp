#include "pch.h"

#include "application.h"
#include "window.h"
#include "renderer.h"
#include "arena.h"
#include "ui.h"
#include "viewport.h"
#include "vulkan_core.h"

#ifdef _WIN32
Application::Application() {
	vulkanContext = GetArena().add<VulkanContext>(); 
}

bool Application::init(int nCmdShow) {
	if (!Window::create(nCmdShow)) {
		return false;
	}

	if (!Renderer::init(*vulkanContext)) {
		return false;
	}

	UI::init(Window::getHandle(), *vulkanContext);
	Viewport::createViewportTarget(*vulkanContext, vulkanContext->viewport, 1, 1);

	return true;
}


void Application::run()
{
	while (!Window::close()) {
		Window::pollEvents();
		if (Window::close() || Window::isMinimized()) {
			continue;
		}

		Renderer::prepareFrame(*vulkanContext);

		UI::begin();
		UI::draw(vulkanContext->viewport);

		Renderer::draw(*vulkanContext);
	}
}

void Application::end()
{
	vkDeviceWaitIdle(vulkanContext->device);
	Viewport::destroyViewportTarget(*vulkanContext, vulkanContext->viewport);
	UI::destroy(*vulkanContext);
	Renderer::destroy(*vulkanContext);
}

#else 

#endif