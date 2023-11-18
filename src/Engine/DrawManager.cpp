#include "Header.h"
#include "DrawManager.h"
#include "Direct2DContext.h"

DrawManager::DrawManager()
{
	m_drawObjects.reserve(MAX_2D_OBJECTS);
}

DrawManager::~DrawManager()
{
}

void DrawManager::Draw()
{
	for (int i = 0; i < m_drawObjects.size(); i++)
	{
		if(m_drawObjects[i] && m_drawObjects[i]->IsVisible())
			m_drawObjects[i]->Draw();
	}
}
