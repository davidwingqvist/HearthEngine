#include "Header.h"
#include "DragPointer.h"
#include "ResourceManager.h"

DragPointer::DragPointer()
{
	
}

void DragPointer::Draw(const recs::Entity& currentEntity)
{
	if (m_currentRegistry && currentEntity != recs::NULL_ENTITY)
	{

	}
}

void DragPointer::SetRegistry(recs::recs_registry* registry)
{
	m_currentRegistry = registry;
}

void DragPointer::StartUp()
{
	m_pointerModel = ResourceManager::Get().GetResource<Model3D>("pointer.obj");
}
