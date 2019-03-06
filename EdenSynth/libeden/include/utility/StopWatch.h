#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.03.2019
/// 
#include <chrono>

namespace eden::utility
{
	/// <summary>
	/// Time-measuring utility. Measures time elapsed from object 
	/// construction to destruction.
	/// </summary>
	class StopWatch
	{
	public:
		/// <param name="timeElapsed">duration to store the measured time in</param>
		StopWatch(std::chrono::nanoseconds& timeElapsed);
		~StopWatch();

	private:
		std::chrono::nanoseconds& _timeElapsed;
		std::chrono::high_resolution_clock::time_point _startTime;
	};
}
