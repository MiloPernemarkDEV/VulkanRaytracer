#pragma once
#include "vulkan_core.h"

#ifdef _WIN32 
class Application {
public:
	Application();
	[[nodiscard]] bool init(int nCmdShow) const;
	void run() const;
	void end() const;
private:
	VulkanContext* vulkanContext = nullptr;
};

#else 
class Application {
public:
	Application();
};
#endif
