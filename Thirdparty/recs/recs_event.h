#pragma once
#include <functional>

namespace recs
{
	template<typename T>
	class recs_event
	{
	private:

		std::function<void()> m_event;

	public:

		recs_event()
		{
			m_event = nullptr;
		}

		~recs_event() = default;

		void SetEvent(const std::function<void()>& event)
		{
			this->m_event = event;
		}

		void RunEvent()
		{
			if (m_event)
				m_event();
		}
	};
}