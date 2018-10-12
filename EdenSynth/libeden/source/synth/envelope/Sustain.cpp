/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/Sustain.h"

namespace eden::synth::envelope
{
	Sustain::Sustain()
		: EnvelopeSegment(0)
	{
	}

	SampleType Sustain::updateAndReturnPerSampleGain()
	{
		return 0.0;
	}

	bool Sustain::hasEnded(SampleType currentGain)
	{
		return false;
	}

	void Sustain::reset()
	{
	}
}
