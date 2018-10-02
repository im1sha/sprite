#pragma once

#include "Sprite.h"

class Handler
{
public:

	Handler(Sprite* sprite);
	~Handler();

	static POINT getPosition(LPARAM lParam);

private:
	Sprite* sprite_;  

};