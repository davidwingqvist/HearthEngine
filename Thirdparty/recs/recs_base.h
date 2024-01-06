#pragma once
#include <queue>
#include "recs_pch.h"
#include "recs_component_registry.h"
#include "recs_component_array.h"
#include "recs_event_handler.h"
#include "recs_thread.h"


/*

	TODO: 
	* Optimize groups.
	* Create dynamic groups. -- These should be able to be saved, but might affect performance.
	* Create "Duel" function. -- This one takes in two entities and double the components, it is meant to be an easy way to loop through each entity against each other.

	Ideas:
	* Limit some entities to not be looped through?
*/

namespace recs
{

	template<typename T>
	class recs_entity_handle;

	template<class... views>
	class recs_entity_group;

	/*
		A registry that holds each available entities.
		This is the base class of the ECS system.
		Usually only one instance of this object should be present.
		Take a look at the functions present to get familiarized with the system.
	*/
	class recs_registry
	{
	private:

		std::queue<recs::Entity> m_availableEntities;
		std::vector<Entity> m_activeEntities;
		recs_component_registry m_componentRegistry;
		recs_thread_pool m_threadpool;
		bool m_useOpenMp = false;
		

		/*
			Storage of all different types of events.
		*/
		std::unordered_map<size_t, std::shared_ptr<recs_event_handler_interface>> m_eventHandler;

		size_t m_size = DEFAULT_MAX_ENTITIES;

		template<typename T>
		recs_event_handler<T>* GetEventHandler()
		{
			const size_t type = typeid(T).hash_code();

			// Create event handler if not already existing.
			if (m_eventHandler.find(type) == m_eventHandler.end())
				m_eventHandler.insert({ type, std::make_shared<recs_event_handler<T>>() });

			return dynamic_cast<recs_event_handler<T>*>(m_eventHandler.at(type).get());
		}

	public:

		// Creates a registry, allocated assigned(size) amount of entities.
		recs_registry(const size_t& size = DEFAULT_MAX_ENTITIES);

		~recs_registry();

		// Generates an entity and outputs it. Another copy is stored inside.
		Entity CreateEntity();

		/*
			Add a component to an entity, outputs a pointer to the newely created component.
			This will also register a component if it doesn't already exist.
		*/
		template<typename T>
		T* AddComponent(const Entity& entity);

		/*
			Replace an already existing component with input to said entity.
			If the entity didn't have said component then simply add it to the entity.
		*/
		template<typename T>
		T* AddOrReplaceComponent(const Entity& entity, const T& component);

		/*
			Replaces said component from input entity.
			If it didn't exist on the entity then a nullptr will be returned.
		*/
		template<typename T>
		T* ReplaceComponent(const Entity& entity, const T& component);

		// Get specific component from an entity. Return nullptr if it doesn't exist.
		template<typename T>
		T* GetComponent(const Entity& entity) const;


		// Return a bool that indicates whether an entity contains said component.
		template<typename T>
		const bool HasComponent(const Entity& entity) const;


		/*
			Remove specific component from an entity.
		*/
		template<typename T>
		void RemoveComponent(const Entity& entity);


		/* Destroy an entityand erase all the components from it.
		*  This function is slow, so try to avoid it.
		*/
		void DestroyEntity(const Entity& entity);


		// Register component with DEFAULT_MAX_ENTITIES size as default size.
		template<typename T>
		void RegisterComponent(const size_t& size = DEFAULT_MAX_ENTITIES, const bool& shouldCopy = false);

		// Register an event that can be called with the RunEvent() function.
		template<typename T, T eventType>
		void RegisterEvent(const std::function<void()>& func);


		// Run a specific event linked to the template and template object.
		template<typename T, T eventType>
		void RunEvent();


		/*
			Register a function that will execute each time a component is created.
		*/
		template<typename T>
		void RegisterOnCreate(std::function<void(const Entity&, T&)> func);


		/*
			Register a function that will execute each time Update() is called.
		*/
		template<typename T>
		void RegisterOnUpdate(std::function<void(const Entity&, T&)> func);


		/*
			Register a function that will execute each time the entity or component is destroyed.
		*/
		template<typename T>
		void RegisterOnDestroy(std::function<void(const Entity&, T&)> func);

		// Get a view of a specific component.
		template<typename T>
		recs_entity_handle<T> View() noexcept
		{
			recs_entity_handle<T> view(this);

			return view;
		}

		// Get a group of specific components.
		template<class... types>
		recs_entity_group<types...> Group();


		/*
		Call this function to initiate an update call to each component with 
			a valid update function.
		*/
		void Update();

		recs_component_registry& GetComponentRegistry()
		{
			return m_componentRegistry;
		}

		/*
			Starts a pool of numThreads amount of threads.
		*/
		void Pool(const size_t& numThreads = std::thread::hardware_concurrency() - 1);

		/*
			Activates OpenMP support for View and Group function
		*/
		void UseOpenMP(const bool& boolean);

		/*
			Check if the registry is currently using OpenMP
		*/
		const bool& IsUsingOpenMP() const;
	};

	template<typename T>
	inline T* recs_registry::AddComponent(const Entity& entity)
	{
		return m_componentRegistry.AddComponentToEntity<T>(entity);
	}

	template<typename T>
	inline T* recs_registry::AddOrReplaceComponent(const Entity& entity, const T& component)
	{
		return m_componentRegistry.AddOrReplaceComponent<T>(entity, component);
	}

	template<typename T>
	inline T* recs_registry::ReplaceComponent(const Entity& entity, const T& component)
	{
		return m_componentRegistry.ReplaceComponent<T>(entity, component);
	}

	template<typename T>
	inline T* recs_registry::GetComponent(const Entity& entity) const
	{
		T* compArray = m_componentRegistry.GetComponentArray<T>();
		if (compArray == nullptr)
			return nullptr;

		const Link& link = m_componentRegistry.GetEntityLink<T>();

		if (link.find(entity) == link.end())
			return nullptr;

		return &compArray[link.at(entity)];
	}

	template<typename T>
	inline const bool recs_registry::HasComponent(const Entity& entity) const
	{
		T* comp = this->GetComponent<T>(entity);

		return comp ? true : false;
	}

	template<typename T>
	inline void recs_registry::RemoveComponent(const Entity& entity)
	{
		m_componentRegistry.RemoveComponentFromEntity<T>(entity);
	}

	template<typename T>
	inline void recs_registry::RegisterComponent(const size_t& size, const bool& shouldCopy)
	{
		// Not allowed to have more components than entities. Shrinking to m_size.
		if (m_size < size)
		{
			m_componentRegistry.RegisterComponent<T>(m_size);
			std::cout << "RECS [NOTICE]: Size was higher than available entities, size was adjusted to: " << m_size << " components.\n";
			return;
		}

		m_componentRegistry.RegisterComponent<T>(size);
	}

	template<typename T>
	inline void recs_registry::RegisterOnCreate(std::function<void(const Entity&, T&)> func)
	{
		m_componentRegistry.AssignOnCreateToComponent<T>(func);
	}

	template<typename T>
	inline void recs_registry::RegisterOnUpdate(std::function<void(const Entity&, T&)> func)
	{
		m_componentRegistry.AssignOnUpdateToComponent(func);
	}

	template<typename T>
	inline void recs_registry::RegisterOnDestroy(std::function<void(const Entity&, T&)> func)
	{
		m_componentRegistry.AssignOnDestroyToComponent(func);
	}

	template<typename T, T eventType>
	inline void recs_registry::RegisterEvent(const std::function<void()>& func)
	{
		this->GetEventHandler<T>()->RegisterEvent<T>(eventType, func);
	}

	template<typename T, T eventType>
	inline void recs_registry::RunEvent()
	{
		this->GetEventHandler<T>()->RunEvent<T, eventType>();
	}

	template<class ...types>
	inline recs_entity_group<types...> recs_registry::Group()
	{
		return std::move(recs_entity_group<types...>(this));
	}
}

/*

	Wrappers that are used for recs_registry.

*/
namespace recs
{

	/*
		This is a handle that lists all entities linked to a specific component.
	*/

	template<typename T>
	class recs_entity_handle
	{
	protected:

		const Link& m_linker;
		const std::vector<EntityLink>& m_entityLinker;
		unsigned short m_pos = 0;
		T* m_componentArray;
		bool m_useOpenMP = false;

	public:

		recs_entity_handle(recs::recs_registry* registry)
			: m_linker(registry->GetComponentRegistry().GetEntityLink<T>()), m_componentArray(registry->GetComponentRegistry().GetComponentArray<T>()), m_entityLinker(registry->GetComponentRegistry().GetEntityLinks<T>())
		{
			m_pos = 0;
			m_useOpenMP = registry->IsUsingOpenMP();
		}

		/*
		* The first argument should be 'const Entity& entity'
			loop through each component and execute the function.
			Check the other overloaded functions.
		*/
		virtual void ForEach(const std::function<void(const Entity&, T&)>&& func);

		// loop through each component and execute the function.
		virtual void ForEach(const std::function<void(T&)>&& func);

		/*
			Returns the value at pos, and will not increment the check value.
		*/
		virtual T& Next(const Entity& entity)
		{
			return m_componentArray[m_linker.at(entity)];
		}

		/*
			Reset the Next value to 0.
		*/
		virtual void ResetNext()
		{
			m_pos = 0;
		}

		/*
			Reset the Next value with specific position value.
		*/
		virtual void ResetNext(const unsigned short&& p_value)
		{
			m_pos = p_value;
		}

		virtual const size_t Size() const noexcept
		{
			return m_linker.size();
		}

		virtual const Link& GetLink() const
		{
			return m_linker;
		}

		virtual const std::vector<EntityLink>& GetEntityLink() const
		{
			return m_entityLinker;
		}

		virtual bool HasEntity(const Entity& entity) const
		{
			if (m_linker.find(entity) != m_linker.end())
				return true;
			
			return false;
		}
	};

	template<typename T>
	inline void recs_entity_handle<T>::ForEach(const std::function<void(const Entity&, T&)>&& func)
	{
		for (int i = 0; i < m_entityLinker.size(); i++)
		{
			func(m_entityLinker[i].entity, m_componentArray[m_entityLinker[i].pos]);
		}
	}

	template<typename T>
	inline void recs_entity_handle<T>::ForEach(const std::function<void(T&)>&& func)
	{
		int i = 0;

		if (m_useOpenMP)
		{
			#pragma omp parallel for schedule(static) private(i)
			for (i = 0; i < m_entityLinker.size(); i++)
			{
				func(m_componentArray[m_entityLinker[i].pos]);
			}
		}
		else
		{
			for (i = 0; i < m_entityLinker.size(); i++)
			{
				func(m_componentArray[m_entityLinker[i].pos]);
			}
		}
	}

	/*
		This is a class that holds several different types of components that can be iterated over.
		Please discard any created instance after you are done with them, as in: Don't save any of these since they will be outdated in the next loop.
	*/
	template<class... views>
	class recs_entity_group : public recs_entity_handle<views>...
	{
	private:

		const unsigned int m_amount;
		const Entity_Group m_list;

		Link const* GetLowestLink()
		{
			size_t size = INT_MAX;
			((size > dynamic_cast<recs_entity_handle<views>*>(this)->Size() ? size = dynamic_cast<recs_entity_handle<views>*>(this)->Size() : size = size), ...);

			Link const* link = nullptr;
			((size == dynamic_cast<recs_entity_handle<views>*>(this)->Size() ? link = &dynamic_cast<recs_entity_handle<views>*>(this)->GetLink() : link = link), ...);

			assert(link != nullptr && "RECS [ASSERT ERROR]: Couldn't find the lowest link.");

			return link;
		}

		const Entity_Group GenerateList()
		{
			Link const* lowest = this->GetLowestLink();

			Entity_Group group;

			for (auto& link : *lowest)
			{
				unsigned short everyHas = 0;
				((dynamic_cast<recs_entity_handle<views>*>(this)->HasEntity(link.first) == true ? everyHas++ : everyHas--), ...);

				if (everyHas == m_amount)
					group.push_back(link.first);
			}

			return group;
		}

	public:

		recs_entity_group(recs::recs_registry* registry)
			:recs_entity_handle<views>(registry)..., m_amount(sizeof...(views)), m_list(this->GenerateList())
		{
			static_assert(sizeof...(views) > 1, "RECS [ASSERT ERROR]: Groups needs to be more than one.");
		}

		/*
			Goes through each entity that contain the connected components and exectues the function.

			Rememeber: If one of the components doesn't exist for the entity or system, this function will not run.
		*/
		void ForEach(const std::function<void(views&...)>& func) noexcept
		{

			for(auto& entity : m_list)
				func(dynamic_cast<recs_entity_handle<views>*>(this)->Next(entity)...);
		}

		/*
			Goes through each entity that contain the connected components and exectues the function.

			Rememeber: If one of the components doesn't exist for the entity or system, this function will not run.
		*/
		void ForEach(const std::function<void(const Entity&, views&...)>& func) noexcept
		{

			for (auto& entity : m_list)
				func(entity, dynamic_cast<recs_entity_handle<views>*>(this)->Next(entity)...);
		}

		const size_t Size() noexcept
		{
			return m_list.size();
		}

		/*
			Not implemented yet
		*/
		void Next() const
		{
			std::cout << "This function is not implemented.\n";
		}

		/*
			Not implemented yet
		*/
		void ResetNext() const
		{
			std::cout << "This function is not implemented.\n";
		}

		/*
			Return the amount of components connected to the group.
		*/
		const unsigned int& Amount() const
		{
			return m_amount;
		}
	};
}