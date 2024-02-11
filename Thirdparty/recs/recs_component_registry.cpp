#include "recs_pch.h"
#include "recs_component_registry.h"

namespace recs
{
	const std::vector<EntityLink>& recs_component_registry::GetEntityLinks(const size_t& id) const
	{
		return m_componentArrays.at(id).get()->GetLinks();
	}

	void recs::recs_component_registry::UpdateAllComponents()
	{
		for (auto& comp : m_componentArrays)
		{
			comp.second.get()->UpdateComponents();
		}
	}

	void recs_component_registry::Clear()
	{
		for (auto& comp : m_componentArrays)
		{
			comp.second.get()->Clear();
		}
	}

	void* recs_component_registry::GetComponentArray(const size_t& compArrId) const
	{
		return m_componentArrays.at(compArrId).get()->GetData();
	}

	void recs_component_registry::LinkEntityToPos(const size_t& compArrId, const Entity& entity, const size_t& pos)
	{
		recs_component_array_interface* intf = m_componentArrays.at(compArrId).get();
		
		if (intf)
		{
			intf->LinkEntityToPos(entity, pos);
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
