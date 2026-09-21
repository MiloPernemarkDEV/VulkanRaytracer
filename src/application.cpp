#include "pch.h"

#include "application.h"
#include "window.h"
#include "renderer.h"
#include "arena.h"
#include "ui.h"

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
	

	return true;
}


void Application::run()
{
	while (!Window::close()) {
		Window::pollEvents();
		if (Window::close() || Window::isMinimized()) {
			continue;
		}

		UI::begin();
		UI::draw();

		Renderer::draw(*vulkanContext);
	}
}

void Application::end()
{
	UI::shutdown(*vulkanContext);
	Renderer::terminate(*vulkanContext);
}

#else 

#endif