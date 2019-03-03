/// 
/// \author Jan Wilczek
/// \date 03.03.2019
/// 
#include "utility/threading/ThreadPool.h"

namespace eden::utility::threading
{
	ThreadPool::ThreadPool()
		: _done(false)
		, _joiner(_threads)
	{
		const auto threadCount = std::thread::hardware_concurrency();

		try
		{
			for (auto i = 0u; i < threadCount; ++i)
			{
				_threads.emplace_back(&ThreadPool::workerThread, this);
			}
		}
		catch(...)
		{
			_done = true;
			throw;
		}
	}

	ThreadPool::~ThreadPool()
	{
		_done = true;
	}

	template <typename FunctionType>
	void ThreadPool::submit(FunctionType f)
	{
		_workQueue.push(std::function<void()>(f));
	}

	void ThreadPool::workerThread()
	{
		while (!_done)
		{
			std::function<void()> task;
			if (_workQueue.try_pop(task))
			{
				task();
			}
			else
			{
			std::this_thread::yield();
			}
		}
	}
}
