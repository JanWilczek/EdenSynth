///
/// \author Jan Wilczek
/// \date 03.03.2019
///
#include "utility/threading/JoinThreads.h"

namespace eden::utility::threading {
JoinThreads::JoinThreads(std::vector<std::thread>& threads)
    : _threads(threads) {}

JoinThreads::~JoinThreads() {
  for (auto& thread : _threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}
}  // namespace eden::utility::threading
