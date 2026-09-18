#include <iostream>

#ifdef _WIN32 
class Application {
public:
	Application();
	bool init(int nCmdShow); 
	void run(); 
	void end(); 
private:
};

#else 
class Application {
public:
	Application();
};
#endif
