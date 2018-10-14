/// 
/// \author Jan Wilczek
/// \date 14.10.18
/// 
#include <cmath>
#include <limits>
#include "synth/envelope/ExponentialGain.h"
#include <algorithm>

namespace eden::synth::envelope
{
	// TODO: This method is incomplete.
	void ExponentialGain::applyAndUpdateGain(SampleType& currentGain)
	{
		if (currentGain <= 0.0)
		{
			//currentGain = static_cast<SampleType>(std::numeric_limits<float>::epsilon());
			currentGain = MINIMUM_LEVEL;
		}
		else
		{
			currentGain *= _multiplier;
		}
	}

	// TODO: This method is incomplete.
	void ExponentialGain::calculateGain(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
	{
		constexpr double millisecondsInASecond = 1000.0;
		const auto durationInSamples = (duration.count() / millisecondsInASecond) * sampleRate;

		initialLevel = std::max(initialLevel, MINIMUM_LEVEL);
		finalLevel = std::max(finalLevel, MINIMUM_LEVEL);

		_multiplier = static_cast<SampleType>((20.0 / durationInSamples) * std::log10(finalLevel / initialLevel));
	}
}
