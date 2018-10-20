/// 
/// \author Jan Wilczek
/// \date 14.10.18
/// 
#include "synth/envelope/LinearGain.h"
#include "utility/TimeSampleConverter.h"

namespace eden::synth::envelope
{
	void LinearGain::applyAndUpdateGain(SampleType& currentLevel)
	{
		currentLevel += _gainPerSample;
	}

	void LinearGain::calculateGain(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
	{
		const auto durationInSamples = utility::TimeSampleConverter::timeToSamples(duration, sampleRate);
		_gainPerSample = (finalLevel - initialLevel) / durationInSamples;
	}
}
