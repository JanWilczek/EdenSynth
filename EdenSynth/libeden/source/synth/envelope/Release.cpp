/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/Release.h"

namespace eden::synth::envelope
{
	Release::Release(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
		: EnvelopeSegment(sampleRate, duration, initialLevel, finalLevel)
	{
		calculatePerSampleGain();
	}

	bool Release::hasEnded(SampleType currentGain)
	{
		return currentGain <= _finalLevel;
	}

	void Release::reset()
	{
	}
}
