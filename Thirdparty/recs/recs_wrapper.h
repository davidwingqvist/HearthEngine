#pragma once

//namespace recs
//{
//	class recs_registry;
//
//	template<typename T>
//	struct HandleLink
//	{
//		const EntityLink& link;
//		T& component;
//
//		HandleLink(const EntityLink& linker, T& comp)
//			:link(linker), component(comp)
//		{
//
//		}
//	};
//
//	/*
//		This is a handle that lists all entities linked to a specific component.
//	*/
//
//	template<typename T>
//	class recs_entity_handle
//	{
//	private:
//
//		const std::vector<EntityLink>& m_linker;
//		T* m_componentArray = nullptr;
//		size_t m_pos = 0;
//
//	public:
//
//		recs_entity_handle(const std::vector<EntityLink>& linker, void* compArray)
//			: m_linker(linker), m_componentArray((T*)compArray)
//		{
//			//m_componentArray = compArray;
//			m_pos = 0;
//		}
//
//		recs_entity_handle(recs::recs_registry* registry)
//			: m_linker(registry->GetComponentRegistry().GetEntityLinks<T>()), m_componentArray(registry->GetComponentRegistry().GetComponentArray<T>())
//		{
//			m_pos = 0;
//		}
//
//		/*
//		* The first argument should be 'const Entity& entity'
//			loop through each component and execute the function.
//			Check the other overloaded functions.
//		*/
//		void ForEach(const std::function<void(const Entity&, T&)>&& func);
//
//		// loop through each component and execute the function.
//		void ForEach(const std::function<void(T&)>&& func);
//
//		/*
//			Returns the next value in the string of components.
//		*/
//		HandleLink<T>&& Next() noexcept
//		{
//			HandleLink<T> next(m_linker[m_pos], m_componentArray[m_linker[m_pos].pos]);
//
//			if (m_pos > m_linker.size())
//				m_pos = 0;
//			else
//				m_pos += 1;
//
//			return std::move(next);
//		}
//
//		/*
//			Reset the Next value to 0.
//		*/
//		void ResetNext()
//		{
//			m_pos = 0;
//		}
//
//		/*
//			Reset the Next value with specific position value.
//		*/
//		void ResetNext(const size_t&& p_value)
//		{
//			m_pos = p_value;
//		}
//
//		const size_t& GetSize() const
//		{
//			return m_linker.size();
//		}
//	};
//
//	template<typename T>
//	inline void recs_entity_handle<T>::ForEach(const std::function<void(const Entity&, T&)>&& func)
//	{
//		for (auto& link : m_linker)
//			func(link.entity, m_componentArray[link.pos]);
//	}
//
//	template<typename T>
//	inline void recs_entity_handle<T>::ForEach(const std::function<void(T&)>&& func)
//	{
//		for (auto& link : m_linker)
//			func(m_componentArray[link.pos]);
//	}
//
//	template<class... views>
//	class recs_entity_group : public recs_entity_handle<views>...
//	{
//	public:
//
//		recs_entity_group(recs::recs_registry* registry)
//			:recs_entity_handle<views...>(registry)
//		{
//
//		}
//
//	};
//}