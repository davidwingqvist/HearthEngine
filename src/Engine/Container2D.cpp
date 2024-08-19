#include "Header.h"
#include "Container2D.h"

Container::Container()
{

}

void Container::Draw()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] && m_children[i]->m_isVisible)
		{
			m_children[i]->Draw();
		}
	}
}

void Container::Update()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] && m_children[i]->m_isVisible)
		{
			m_children[i]->Update();
		}
	}
}
