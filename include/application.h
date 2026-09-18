#include <iostream>
#include "renderer.h"

#ifdef _WIN32 
class Application {
public:
	Application();
	bool init(int nCmdShow); 
	void run(); 
	void end(); 
private:
	VulkanContext* vulkanContext; 
};

#else 
class Application {
public:
	Application();
};
#endif
