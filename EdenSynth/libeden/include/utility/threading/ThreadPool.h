#pragma once
/// 
/// \author Jan Wilczek
/// \date 03.03.2019
/// 
#include <atomic>
#include <vector>
#include <thread>
#include "utility/threading/ThreadSafeQueue.h"
#include "utility/threading/JoinThreads.h"

namespace eden::utility::threading
{
	/// <summary>
	/// Thread pool implementation proposed by Anthony Williams
	/// in "C++ Concurrency in Action".
	/// </summary>
	class ThreadPool
	{
	public:
		ThreadPool();
		~ThreadPool();

		template <typename FunctionType>
		void submit(FunctionType f);

	private:
		void workerThread();

		std::atomic<bool> _done;
		ThreadSafeQueue _workQueue;
		std::vector<std::thread> _threads;
		JoinThreads _joiner;
	};
}