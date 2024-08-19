#pragma once
#include "Canvas.h"

const size_t MAX_2D_OBJECTS = 250;

/*

	Manager for drawing 2D objects on the screen.

*/
class DrawManager
{
private:

	std::vector<Canvas*> m_drawObjects;

public:

	DrawManager();
	~DrawManager();

	void Draw();

	void Update();


};