#pragma once
#include "recs_pch.h"
#include <iostream>

namespace recs
{
	class recs_component_array_interface
	{
	public:
		virtual ~recs_component_array_interface() = default;
		virtual void RemoveEntity(const recs::Entity& entity) = 0;
		virtual void UpdateComponents() = 0;
	};

	template<typename T>
	class recs_component_array : public recs_component_array_interface
	{
	private:

		std::vector<T> m_components;
		//T* m_components;
		size_t m_size = DEFAULT_MAX_ENTITIES;
		std::queue<size_t> m_availableComponents;
		std::vector<EntityLink> m_activeComponents;
		std::unordered_map<size_t, Entity> m_posToEntity;
		Link m_entityToPos;
		std::function<void(const Entity&, T&)> m_onCreateFunction;
		std::function<void(const Entity&, T&)> m_onDestroyFunction;
		std::function<void(const Entity&, T&)> m_onUpdateFunction;

	public:

		recs_component_array()
		{
			m_components = std::vector<T>(m_size, T());
			//m_components = new T[m_size];
			for (size_t i = 0; i < m_size; i++)
			{
				m_availableComponents.push(i);
			}
			m_activeComponents.reserve(m_size);
		}

		recs_component_array(const size_t& size)
		{
			m_size = size;
			m_components = std::vector<T>(m_size, T());
			//m_components = new T[m_size];
			for (size_t i = 0; i < m_size; i++)
			{
				m_availableComponents.push(i);
			}
			m_activeComponents.reserve(m_size);
		}

		~recs_component_array()
		{
			//if (m_components)
			//	delete m_components;
		}

		// Assign an on create function that is called each time a component is created.
		void AssignOnCreate(std::function<void(const Entity&, T&)> func)
		{
			m_onCreateFunction = func;
		}

		// Assign an on destroy function that is called each time a component is destroyed.
		void AssignOnDestroy(std::function<void(const Entity&, T&)> func)
		{
			m_onDestroyFunction = func;
		}

		// Assign an on update function that is called each time recs::recs_registry::Update() is called.
		void AssignOnUpdate(std::function<void(const Entity&, T&)> func)
		{
			m_onUpdateFunction = func;
		}

		const Link& GetEntityLink() const
		{
			return m_entityToPos;
		}

		// Unsafe GET request that returns a pointer to a component.
		template<typename T>
		T* GetComponentFromEntity(const Entity& entity)
		{
			return &m_components[m_entityToPos[entity]];
		}

		template<typename T>
		T* GetComponentFromEntitySafe(const Entity& entity)
		{
			if (m_entityToPos.find() != m_entityToPos.end())
				return &m_components[m_entityToPos[entity]];
			else
				return nullptr;
		}

		// Link a component to the chosen entity.
		template<typename T>
		T* LinkComponentToEntity(const Entity& entity)
		{
			if (m_availableComponents.empty())
			{
				std::cout << "RECS [Warning!]: Tried to link entity: " << entity <<  " to a component array which was full!\n";
				return nullptr;
			}

			const size_t pos = m_availableComponents.front();
			m_availableComponents.pop();

			EntityLink newLink = { entity, pos };
			m_activeComponents.push_back(newLink);

			m_posToEntity[pos] = entity;
			m_entityToPos[entity] = pos;

			if (m_onCreateFunction)
				m_onCreateFunction(entity, m_components[pos]);

			return &m_components[pos];
		}

		template<typename T>
		T* LinkComponentToEntity(const Entity& entity, const T& component)
		{

			if (m_entityToPos.find(entity) != m_entityToPos.end())
			{
				// Replace the component.
				m_components[m_entityToPos.at(entity)] = component;


				return &m_components[m_entityToPos.at(entity)];
			}

			// Add the component.
			const size_t pos = m_availableComponents.front();
			m_availableComponents.pop();

			EntityLink newLink = { entity, pos };
			m_activeComponents.push_back(newLink);

			m_posToEntity[pos] = entity;
			m_entityToPos[entity] = pos;

			if (m_onCreateFunction)
				m_onCreateFunction(entity, m_components[pos]);

			return &m_components[pos];
		}

		template<typename T>
		T* ReplaceComponentToEntity(const Entity& entity, const T& component)
		{
			if (m_entityToPos.find(entity) != m_entityToPos.end())
			{
				// Replace the component.
				m_components[m_entityToPos.at(entity)] = component;


				return &m_components[m_entityToPos.at(entity)];
			}

			return nullptr;
		}

		// Return the size of the component array.
		const size_t& GetSize() const
		{
			return m_size;
		}

		// Get a pointer towards the raw array.
		T* GetArray()
		{
			return &m_components[0];
		}

		// Return a vector of registered entities to the component array.
		const std::vector<EntityLink>& GetRegisteredComponents() const
		{
			return m_activeComponents;
		}

		// Remove an entity from the component array.
		virtual void RemoveEntity(const Entity& entity) override
		{

			if (m_onDestroyFunction)
				m_onDestroyFunction(entity, m_components[m_entityToPos[entity]]);

			m_entityToPos.erase(entity);
			m_posToEntity.erase(entity);

			m_availableComponents.push(entity);

			for (size_t i = 0; i < m_activeComponents.size(); i++)
			{
				if (m_activeComponents[i].entity == entity)
				{
					m_activeComponents.erase(m_activeComponents.begin() + i);
					return;
				}
			}

			std::cout << "RECS [WARNING!]: Tried to remove a component from an entity: " << entity << " that doesn't have said component.\n";
		}

		// Call the update function of each component that is used by an entity.
		virtual void UpdateComponents() override
		{
			if (!m_onUpdateFunction)
				return;

			for (auto& link : m_activeComponents)
			{
				m_onUpdateFunction(link.entity, m_components[link.pos]);
			}
		}

	};
}