#include <iostream>
#include "vulkan_core.h"

#ifdef _WIN32 
class Application {
public:
	Application();
	bool init(int nCmdShow); 
	void run(); 
	void end(); 
private:
	VulkanContext* vulkanContext = nullptr;
};

#else 
class Application {
public:
	Application();
};
#endif
