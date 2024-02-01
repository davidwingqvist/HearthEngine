#pragma once

namespace recs
{
	class recs_registry;

	class recs_state_handler
	{
	private:

		recs_registry* m_registry = nullptr;

	public:

		recs_state_handler(recs_registry* registryRef = nullptr);
	};
}

