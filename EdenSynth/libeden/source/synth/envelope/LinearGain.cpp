/// 
/// \author Jan Wilczek
/// \date 14.10.18
/// 
#include "synth/envelope/LinearGain.h"

namespace eden::synth::envelope
{
	void LinearGain::applyAndUpdateGain(SampleType& currentLevel)
	{
		currentLevel += _gainPerSample;
	}

	void LinearGain::calculateGain(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
	{
		constexpr double millisecondsInASecond = 1000.0;
		const auto durationInSamples = (duration.count() / millisecondsInASecond) * sampleRate;
		_gainPerSample = (finalLevel - initialLevel) / durationInSamples;
	}
}
