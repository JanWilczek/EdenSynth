/// 
/// \author Jan Wilczek
/// \date 06.03.2019
/// 
#include "utility/StopWatch.h"

namespace eden::utility
{
	StopWatch::StopWatch(std::chrono::nanoseconds& timeElapsed)
		: _timeElapsed(timeElapsed)
		, _startTime(std::chrono::high_resolution_clock::now())
	{}

	StopWatch::~StopWatch()
	{
		_timeElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _startTime);
	}
}
