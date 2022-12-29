#pragma once
///
/// \author Jan Wilczek
/// \date 03.03.2019
///
#include <memory>
#include <mutex>

namespace eden::utility::threading {
/// <summary>
/// A thread-safe queue with locking and waiting.
/// Implementation proposed by Anthony Williams
/// in the book "C++ Concurrency in Action".
/// </summary>
template <typename T>
class ThreadSafeQueue {
public:
  ThreadSafeQueue();
  ThreadSafeQueue(const ThreadSafeQueue& other) = delete;
  ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

  std::shared_ptr<T> tryPop();
  bool tryPop(T& value);
  std::shared_ptr<T> waitAndPop();
  void waitAndPop(T& value);
  void push(T value);
  bool empty();

private:
  struct node {
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;
  };

  node* getTail();
  std::unique_ptr<node> popHead();
  std::unique_lock<std::mutex> waitForData();
  std::unique_ptr<node> waitPopHead();
  std::unique_ptr<node> waitPopHead(T& value);
  std::unique_ptr<node> tryPopHead();
  std::unique_ptr<node> tryPopHead(T& value);

  std::mutex _headMutex;
  std::unique_ptr<node> _head;
  std::mutex _tailMutex;
  node* _tail;
  std::condition_variable _dataCond;
};
}  // namespace eden::utility::threading
