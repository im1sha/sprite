#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include "GraphicDefines.h"
#include "Window.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nCmdShow)
{
	Window* w = new Window(hInstance, nCmdShow);
	w->messageLoop();
	delete w;
	return 0;
}