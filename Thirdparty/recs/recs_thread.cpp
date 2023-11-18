#include "recs_pch.h"
#include "recs_thread.h"

namespace recs
{

	recs_thread_pool::recs_thread_pool()
	{
		m_threads = nullptr;
	}

	recs_thread_pool::~recs_thread_pool()
	{
		m_running = false;

		int stopped = 0;
		while (stopped < m_amountOfThreads)
		{
			stopped = 0;
			for (int i = 0; i < m_amountOfThreads; i++)
			{
				if (m_threads[i].GetStatus() == recs_thread_status::STOPPED)
					stopped++;
			}

		}

		if (m_threads)
			delete[] m_threads;
	}

	void recs_thread_pool::Initialize(const size_t& numThreads)
	{
		m_threads = new recs_thread[numThreads];
		m_amountOfThreads = numThreads;
		m_running = false;
		m_isActive = true;

		for (size_t i = 0; i < numThreads; i++)
		{
			recs_thread thread;
			m_threads[i] = std::move(thread);
			m_threads[i].CreatePooled(this);
		}
	}

	const bool& recs::recs_thread_pool::GetRunStatus() const
	{
		return m_running;
	}

	const bool& recs_thread_pool::GetActive() const
	{
		return m_isActive;
	}

	void recs_thread_pool::AddToQueue(std::function<void(Entity, void*)> func)
	{
		// Todo: Add thread lock

		m_queue.push_back(func);

	}

	std::function<void(Entity, void*)> recs_thread_pool::FetchJob()
	{
		// Todo: Add check if job is ready.

		return nullptr;
	}

	const recs_thread_status& recs_thread::GetStatus() const
	{
		return m_status;
	}

	void recs_thread::SetStatus(const recs_thread_status& status)
	{
		m_status = status;
	}

	recs_thread_pool* recs_thread::GetPool() const
	{
		return m_pool;
	}

	recs_thread::~recs_thread()
	{

	}

	void PoolMain(recs_thread* thread)
	{
		thread->SetStatus(RUNNING);
		while (thread->GetPool()->GetRunStatus())
		{
			std::function<void(Entity, void*)> job = nullptr;

			if (job)
			{

			}
		}
		thread->SetStatus(STOPPED);
	}

	void recs_thread::CreatePooled(recs_thread_pool* control)
	{
		m_pool = control;
		m_thread = new std::thread(PoolMain, this);
	}
}
