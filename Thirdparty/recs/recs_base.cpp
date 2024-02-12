#include "recs_pch.h"
#include "recs_base.h"
#include <iostream>

bool recs::recs_registry::CheckIfExist(const Entity& entId) const
{
	if (m_activeEntities.size() <= 0)
		return false;

	return std::find(m_activeEntities.begin(), m_activeEntities.end(), entId) != m_activeEntities.end();
}

recs::recs_registry::recs_registry(const size_t& size)
	:m_stateHandler(this, &this->GetComponentRegistry())
{
	m_size = size;
	for (Entity i = (Entity)m_size - 1; i != NULL_ENTITY; i--)
	{
		m_availableEntities.push_back(i);
	}
}

recs::Entity recs::recs_registry::CreateEntity()
{
	if (m_availableEntities.empty())
	{
#ifdef _DEBUG
		std::cout << "RECS [Warning!]: Too many entities has been created! Undefined behavior will happen!" << std::endl;
#endif
		return NULL_ENTITY;
	}
	const Entity entity = m_availableEntities.back();
	m_activeEntities.push_back(entity);
	m_availableEntities.pop_back();

	return entity;
}

void recs::recs_registry::CreateEntity(const Entity& entityId)
{
	int spot = 0;
	if (!CheckIfExist(entityId))
	{
		for (int i = 0; i < m_availableEntities.size(); i++)
		{
			if (m_availableEntities[i] == entityId)
			{
				// found, so lets remove it, and add it to active entities.
				goto good_exit;
			}
			spot++;
		}
		// wasn't found :/
		return;
	}

	return;
good_exit:

	m_activeEntities.push_back(entityId);
	m_availableEntities.erase(m_availableEntities.begin() + spot);

	return;
}

recs::recs_registry::~recs_registry()
{

}

void recs::recs_registry::SetDataFolderPath(const std::string& path)
{
	m_stateHandler.SetFolderPath(path);
}

bool recs::recs_registry::SaveData()
{
	return m_stateHandler.SaveData();
}

bool recs::recs_registry::LoadData()
{
	this->Reset();
	return m_stateHandler.LoadData();
}

void recs::recs_registry::Reset()
{
	m_availableEntities.clear();
	m_activeEntities.clear();
	for (Entity i = (Entity)m_size - 1; i != NULL_ENTITY; i--)
	{
		m_availableEntities.push_back(i);
	}

	m_componentRegistry.Clear();
}

const size_t& recs::recs_registry::GetMaxSize() const
{
	return m_size;
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
		m_availableEntities.push_back(freeEntity);
		m_activeEntities.erase(it);
	}
#ifdef _DEBUG
	else
	{

		std::cout << "RECS: Non-existant entity: " << entity <<  " was attempted to be removed.\n";

	}
#endif
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

const std::vector<recs::Entity>& recs::recs_registry::GetEntities() const
{
	return m_activeEntities;
}
