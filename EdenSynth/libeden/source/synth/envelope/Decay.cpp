/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/Decay.h"

namespace eden::synth::envelope
{
	Decay::Decay(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
		: EnvelopeSegment(sampleRate, duration, initialLevel, finalLevel)
	{
		calculatePerSampleGain();
	}

	bool Decay::hasEnded(SampleType currentGain)
	{
		return currentGain <= _finalLevel;
	}

	void Decay::reset()
	{
	}
}