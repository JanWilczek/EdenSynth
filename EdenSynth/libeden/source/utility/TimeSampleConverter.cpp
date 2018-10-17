/// 
/// \author Jan Wilczek
/// \date 16.10.18
/// 
#include "utility/TimeSampleConverter.h"

namespace eden::utility
{
	constexpr unsigned TimeSampleConverter::MILLISECONDS_IN_SECOND;

	unsigned TimeSampleConverter::timeToSamples(std::chrono::seconds duration, double sampleRate)
	{
		return static_cast<unsigned>(duration.count() * sampleRate);
	}

	unsigned TimeSampleConverter::timeToSamples(std::chrono::milliseconds duration, double sampleRate)
	{
		return static_cast<unsigned>(duration.count() * sampleRate / MILLISECONDS_IN_SECOND);
	}

	std::chrono::seconds TimeSampleConverter::samplesToSeconds(unsigned samples, double sampleRate)
	{
		return std::chrono::seconds(static_cast<long long>(samples / sampleRate + 0.5));
	}

	std::chrono::milliseconds TimeSampleConverter::samplesToMilliseconds(unsigned samples, double sampleRate)
	{
		return std::chrono::milliseconds(static_cast<long long>(MILLISECONDS_IN_SECOND * samples / sampleRate + 0.5));
	}
}
