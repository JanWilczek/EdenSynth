/// 
/// \author Jan Wilczek
/// \date 14.10.18
/// 
#include "synth/envelope/NoGain.h"

namespace eden::synth::envelope
{
	void NoGain::applyAndUpdateGain(SampleType& currentLevel)
	{
	}

	void NoGain::calculateGain(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
	{
	}
}
