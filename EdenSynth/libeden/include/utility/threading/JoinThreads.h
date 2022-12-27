#pragma once
///
/// \author Jan Wilczek
/// \date 03.03.2019
///
#include <thread>
#include <vector>

namespace eden::utility::threading {
/// <summary>
/// Simple utility that ensures all threads from a vector are joined
/// when the object is destroyed. Implementation given by Anthony Williams
/// in the book "C++ Concurrency in Action".
/// </summary>
class JoinThreads {
 public:
  explicit JoinThreads(std::vector<std::thread>& threads);
  ~JoinThreads();

 private:
  std::vector<std::thread>& _threads;
};
}  // namespace eden::utility::threading
