#pragma once
#include "recs_event.h"

namespace recs
{
	class recs_event_handler_interface
	{
	private:


	public:

		virtual ~recs_event_handler_interface() = default;

		virtual const size_t& GetEventIdentifier() = 0;
	};

	template<typename T>
	class recs_event_handler : public recs_event_handler_interface
	{
	private:

		std::unordered_map<T, recs_event<T>> m_eventMap;

	public:

		recs_event_handler() = default;
		~recs_event_handler() = default;

		template<typename T>
		void RegisterEvent(const T& eventType, const std::function<void()>& event)
		{
			recs_event<T> newEvent;
			newEvent.SetEvent(event);

			m_eventMap[eventType] = newEvent;
		}

		template<typename T, T eventType>
		void RunEvent()
		{
			m_eventMap[eventType].RunEvent();
		}


		// Inherited via recs_event_handler_interface
		virtual const size_t& GetEventIdentifier() override
		{
			return typeid(T).hash_code();
		}

	};
}