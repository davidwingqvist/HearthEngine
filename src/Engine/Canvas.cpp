#include "Header.h"
#include "Canvas.h"

// next ID controller.
static hrth::BIGINT m_nextId = 0;

Canvas::Canvas()
{
	m_id = ++m_nextId;
}

void Canvas::SetInteractive()
{
	m_isInteractive = true;
}

const hrth::BIGINT& Canvas::GetID() const
{
	return this->m_id;
}
