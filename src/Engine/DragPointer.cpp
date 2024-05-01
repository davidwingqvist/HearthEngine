#include "Header.h"
#include "DragPointer.h"
#include "ResourceManager.h"

DragPointer::DragPointer()
{
	m_pointerModel = ResourceManager::Get().GetResource<Model>("pointer.obj");
}

void DragPointer::Draw(const recs::Entity& currentEntity)
{
	if (m_currentRegistry)
	{

	}
}

void DragPointer::SetRegistry(recs::recs_registry* registry)
{
	m_currentRegistry = registry;
}
