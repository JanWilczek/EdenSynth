///
/// \author Jan Wilczek
/// \date 06.03.2019
///
#include "utility/StopWatchPrinter.h"
#ifdef MSC_VER
#include <windows.h>
#else
#include <iostream>
#endif
#include <sstream>

namespace eden::utility {
StopWatchPrinter::StopWatchPrinter()
    : _stopWatch(std::make_unique<StopWatch>(_duration)) {}

StopWatchPrinter::~StopWatchPrinter() {
  // explicitly delete the StopWatch object to make it store its duration in the
  // _duration member
  _stopWatch.reset(nullptr);

  std::stringstream ss;

  ss << "Time measured: "
     << std::chrono::duration_cast<std::chrono::milliseconds>(_duration).count()
     << " ms." << std::endl;

#ifdef MSC_VER
  OutputDebugStringA(ss.str().c_str());
#else
  std::cerr << ss.str() << std::endl;
#endif
}
}  // namespace eden::utility
