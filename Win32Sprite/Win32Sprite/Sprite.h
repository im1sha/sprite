#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <cmath>
#include <ctime>
#include <windows.h>

class Sprite
{
public:
	Sprite();

	~Sprite();

	void createMoveParameters();

	void invalidate(HWND &hwnd);

	void create(HWND hwnd);

	RECT spriteRect_;
	int x_;
	int y_;
	int radius_;
	int deltaX_;
	int deltaY_;

private:
	const double pi_ = 3.14;
	const int semicircleInDegrees_ = 180;
	const int circleInDegrees_ = 360;
	const int defaultRadius_ = 20;
	const double defaultSpeed_ = 10.0;	
};
