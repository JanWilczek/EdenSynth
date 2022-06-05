#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.03.2019
/// 
#include <memory>
#include "utility/StopWatch.h"

namespace eden::utility
{
	/// <summary>
	/// Class running the stop watch on creation and printing its output 
	/// to debug output on destruction.
	/// </summary>
	class StopWatchPrinter
	{
	public:
		StopWatchPrinter();
		~StopWatchPrinter();

	private:
		std::chrono::nanoseconds _duration;
		std::unique_ptr<StopWatch> _stopWatch;
	};
}
