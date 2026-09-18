#include "pch.h"

#include "application.h"
#include "window.h"

#ifdef _WIN32
Application::Application() {
}

bool Application::init(int nCmdShow) {
	Window::create(nCmdShow);

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
}

#else 

#endif