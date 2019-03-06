/// 
/// \author Jan Wilczek
/// \date 06.03.2019
/// 
#include "utility/StopWatchPrinter.h"
#include <sstream>
#include <windows.h>

namespace eden::utility
{
	StopWatchPrinter::StopWatchPrinter()
		: _stopWatch(std::make_unique<StopWatch>(_duration))
	{
	}

	StopWatchPrinter::~StopWatchPrinter()
	{
		// explicitly delete the StopWatch object
		_stopWatch.reset(nullptr);

		std::stringstream ss;

		ss << "Time measured: "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(_duration).count()
			<< " ms."
			<< std::endl;

		OutputDebugStringA(ss.str().c_str());
	}
}
