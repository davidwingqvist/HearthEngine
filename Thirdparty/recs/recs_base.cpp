#include "recs_pch.h"
#include "recs_base.h"
#include <iostream>

recs::recs_registry::recs_registry(const size_t& size)
{
	m_size = size;
	for (Entity i = 0; i < m_size; i++)
	{
		m_availableEntities.push(i);
	}
}

recs::Entity recs::recs_registry::CreateEntity()
{
	if (m_availableEntities.empty())
		std::cout << "RECS [Warning!]: Too many entities has been created! Undefined behavior will happen!" << std::endl;

	const Entity entity = m_availableEntities.front();
	m_activeEntities.push_back(entity);
	m_availableEntities.pop();

	return entity;
}

recs::recs_registry::~recs_registry()
{

}

void recs::recs_registry::DestroyEntity(const Entity& entity)
{
	m_componentRegistry.EntityRemoved(entity);
	std::vector<Entity>::iterator it = std::find(m_activeEntities.begin(), m_activeEntities.end(), entity);

	if (it != m_activeEntities.cend())
	{
		// Gets the position of the removed entity and removes it
		ptrdiff_t dist = std::distance(m_activeEntities.begin(), it);
		Entity freeEntity = m_activeEntities.at(dist);

		// pushes it back into available entities.
		m_availableEntities.push(freeEntity);
		m_activeEntities.erase(it);
	}
	else
	{
		std::cout << "RECS: Non-existant entity: " << entity <<  " was attempted to be removed.\n";
	}
}

void recs::recs_registry::Update()
{
	m_componentRegistry.UpdateAllComponents();
}

void recs::recs_registry::Pool(const size_t& numThreads)
{
	m_threadpool.Initialize(numThreads);
}

void recs::recs_registry::UseOpenMP(const bool& boolean)
{
	m_useOpenMp = boolean;
}

const bool& recs::recs_registry::IsUsingOpenMP() const
{
	return m_useOpenMp;
}
