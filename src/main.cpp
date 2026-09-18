#include "pch.h"
#include <application.h>

#ifdef _WIN32 
#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
	Application app; 
	try {
		if (!app.init(nCmdShow)) {
			return -1;
		}
		app.run();
		app.end();
	}
	catch (const std::exception& e) {
		MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	return 0;
}

#else 

int main() {


	return 0;
}

#endif
