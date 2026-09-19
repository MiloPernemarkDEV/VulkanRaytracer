#include "pch.h"

#include "application.h"
#include "window.h"
#include "renderer.h"
#include "arena.h"

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
	

	return true;
}

void Application::run()
{
	while (!Window::close()) {
		Window::pollEvents();
	}
}

void Application::end()
{
	Renderer::terminate(*vulkanContext);	
}

#else 

#endif