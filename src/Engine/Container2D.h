#pragma once
#include "Canvas.h"

class Container: public Canvas
{
public:

	Container();

	void Draw() override;

private:

	std::vector<Canvas*> m_children;

};