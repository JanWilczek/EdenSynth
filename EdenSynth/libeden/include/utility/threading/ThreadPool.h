#pragma once
///
/// \author Jan Wilczek
/// \date 03.03.2019
///
#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include "utility/threading/JoinThreads.h"
#include "utility/threading/ThreadSafeQueue.h"

namespace eden::utility::threading {
/// <summary>
/// Thread pool implementation proposed by Anthony Williams
/// in "C++ Concurrency in Action".
/// </summary>
class ThreadPool {
public:
  /// <summary>
  /// Constructs the thread pool with a size equal to the
  /// number returned by the std::thread::hardware_concurrency() function.
  /// </summary>
  ThreadPool();
  ~ThreadPool();

  template <typename FunctionType>
  void submit(FunctionType f) {
    _workQueue.push(std::function<void()>(f));
  }

private:
  void workerThread();

  std::atomic<bool> _done;
  ThreadSafeQueue<std::function<void()>> _workQueue;
  std::vector<std::thread> _threads;
  JoinThreads _joiner;
};
}  // namespace eden::utility::threading
