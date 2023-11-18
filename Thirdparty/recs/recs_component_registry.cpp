#include "recs_pch.h"
#include "recs_component_registry.h"

namespace recs
{

	void recs::recs_component_registry::UpdateAllComponents()
	{
		for (auto& comp : m_componentArrays)
		{
			comp.second.get()->UpdateComponents();
		}
	}

	void recs::recs_component_registry::EntityRemoved(const Entity& entity)
	{
		for (auto& comp : m_componentArrays)
		{
			comp.second.get()->RemoveEntity(entity);
		}
	}

}
