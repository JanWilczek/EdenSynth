/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/Attack.h"

namespace eden::synth::envelope
{
	Attack::Attack(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
		: EnvelopeSegment(sampleRate, duration, initialLevel, finalLevel)
	{
		calculatePerSampleGain();
	}

	bool Attack::hasEnded(SampleType currentGain)
	{
		return currentGain >= _finalLevel;
	}

	void Attack::reset()
	{
	}
}
