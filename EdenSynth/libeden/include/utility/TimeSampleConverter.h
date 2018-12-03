#pragma once
/// 
/// \author Jan Wilczek
/// \date 16.10.18
/// 
#include <chrono>

namespace eden::utility
{
	class TimeSampleConverter
	{
	public:
		/// <summary>
		/// Converts time duration to a number of samples it conveys using given sample rate.
		/// </summary>
		/// <param name="duration"></param>
		/// <param name="sampleRate"></param>
		/// <returns>number of samples corresponding to the given <paramref name="duration"/></returns>
		static unsigned timeToSamples(std::chrono::seconds duration, float sampleRate);

		/// <summary>
		/// Converts time duration to a number of samples it conveys using given sample rate.
		/// </summary>
		/// <param name="duration"></param>
		/// <param name="sampleRate"></param>
		/// <returns>number of samples corresponding to the given <paramref name="duration"/></returns>
		static unsigned timeToSamples(std::chrono::milliseconds duration, float sampleRate);

		/// <summary>
		/// Converts given number of samples to their duration in seconds.
		/// </summary>
		/// <param name="samples"></param>
		/// <param name="sampleRate"></param>
		/// <returns>duration of the given number of <paramref name="samples"/> in seconds</returns>
		static std::chrono::seconds samplesToSeconds(unsigned samples, float sampleRate);

		/// <summary>
		/// Converts given number of samples to their duration in milliseconds.
		/// </summary>
		/// <param name="samples"></param>
		/// <param name="sampleRate"></param>
		/// <returns>duration of the given number of <paramref name="samples"/> in milliseconds</returns>
		static std::chrono::milliseconds samplesToMilliseconds(unsigned samples, float sampleRate);

		static constexpr unsigned MILLISECONDS_IN_SECOND = 1000u;
	};
}
