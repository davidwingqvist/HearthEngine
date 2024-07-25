#include "Header.h"
#include "Canvas.h"

// next ID controller.
static hrth::BIGINT m_nextId = 0;

Canvas::Canvas()
{
	m_id = ++m_nextId;
}

void Canvas::SetVisibility(const bool& visibility)
{
	m_isVisible = visibility;
}

void Canvas::SetInteractive()
{
	m_isInteractive = true;
}

void Canvas::DrawObject(const object2D& object)
{
	switch (object.type)
	{
	case CANVAS_TYPE::PICTURE:

		break;
	default:
		break;
	}
}

const hrth::BIGINT& Canvas::GetID() const
{
	return this->m_id;
}
