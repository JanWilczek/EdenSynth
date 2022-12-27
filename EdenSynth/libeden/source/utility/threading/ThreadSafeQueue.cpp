///
/// \author Jan Wilczek
/// \date 03.03.2019
///
#include "utility/threading/ThreadSafeQueue.h"
#include <functional>

namespace eden::utility::threading {
template <typename T>
ThreadSafeQueue<T>::ThreadSafeQueue()
    : _head(std::make_unique<node>()), _tail(_head.get()) {}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::tryPop() {
  std::unique_ptr<node> oldHead = tryPopHead();
  return oldHead ? oldHead->data : std::shared_ptr<T>();
}

template <typename T>
bool ThreadSafeQueue<T>::tryPop(T& value) {
  std::unique_ptr<node> const oldHead = tryPopHead(value);
  return oldHead.get();
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::waitAndPop() {
  std::unique_ptr<node> const oldHead = waitPopHead();
  return oldHead->data;
}

template <typename T>
void ThreadSafeQueue<T>::waitAndPop(T& value) {
  std::unique_ptr<node> const oldHead = waitPopHead(value);
}

template <typename T>
void ThreadSafeQueue<T>::push(T value) {
  auto newData = std::make_shared<T>(std::move(value));
  auto p = std::make_unique<node>();

  {
    std::lock_guard<std::mutex> tailLock(_tailMutex);
    _tail->data = newData;
    node* const newTail = p.get();
    _tail->next = std::move(p);
    _tail = newTail;
  }

  _dataCond.notify_one();
}

template <typename T>
bool ThreadSafeQueue<T>::empty() {
  std::lock_guard<std::mutex> headLock(_headMutex);
  return (_head.get() == getTail());
}

template <typename T>
typename ThreadSafeQueue<T>::node* ThreadSafeQueue<T>::getTail() {
  std::lock_guard<std::mutex> tailLock(_tailMutex);
  return _tail;
}

template <typename T>
std::unique_ptr<typename ThreadSafeQueue<T>::node>
ThreadSafeQueue<T>::popHead() {
  std::unique_ptr<node> oldHead = std::move(_head);
  _head = std::move(oldHead->next);
  return oldHead;
}

template <typename T>
std::unique_lock<std::mutex> ThreadSafeQueue<T>::waitForData() {
  std::unique_lock<std::mutex> headLock(_headMutex);
  _dataCond.wait(headLock, [&] { return _head.get() != getTail(); });
  return headLock;
}

template <typename T>
std::unique_ptr<typename ThreadSafeQueue<T>::node>
ThreadSafeQueue<T>::waitPopHead() {
  std::unique_lock<std::mutex> headLock(waitForData());
  return popHead();
}

template <typename T>
std::unique_ptr<typename ThreadSafeQueue<T>::node>
ThreadSafeQueue<T>::waitPopHead(T& value) {
  std::unique_lock<std::mutex> headLock(waitForData());
  value = std::move(*_head->data);
  return popHead();
}

template <typename T>
std::unique_ptr<typename ThreadSafeQueue<T>::node>
ThreadSafeQueue<T>::tryPopHead() {
  std::lock_guard<std::mutex> headLock(_headMutex);

  if (_head.get() == getTail()) {
    return std::unique_ptr<node>();
  }

  return popHead();
}

template <typename T>
std::unique_ptr<typename ThreadSafeQueue<T>::node>
ThreadSafeQueue<T>::tryPopHead(T& value) {
  std::lock_guard<std::mutex> headLock(_headMutex);

  if (_head.get() == getTail()) {
    return std::unique_ptr<node>();
  }

  value = std::move(*_head->data);
  return popHead();
}

template class ThreadSafeQueue<std::function<void()>>;
}  // namespace eden::utility::threading
