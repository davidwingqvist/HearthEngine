#pragma once
#include "DrawObject.h"

const size_t MAX_2D_OBJECTS = 250;

/*

	Manager for drawing 2D objects on the screen.

*/
class DrawManager
{
private:

	std::vector<IDrawObject*> m_drawObjects;

public:

	DrawManager();
	~DrawManager();

	void Draw();


};