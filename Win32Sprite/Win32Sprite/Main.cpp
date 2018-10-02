
#include "Window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	Window* window = new Window(hInstance, nCmdShow);

	window->messageLoop();

	delete window;

	return 0;
}
