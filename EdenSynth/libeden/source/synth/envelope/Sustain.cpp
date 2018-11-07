/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/Sustain.h"
#include "synth/envelope/NoGain.h"

namespace eden::synth::envelope
{
	Sustain::Sustain()
		: EnvelopeSegment(0, std::make_unique<NoGain>())
	{
	}

	bool Sustain::hasEnded(SampleType currentGain)
	{
		return false;
	}

	void Sustain::setDuration(std::chrono::milliseconds duration)
	{	
	}

	void Sustain::setGainCurve(std::unique_ptr<ISegmentGain> envelopeGain)
	{
	}
}
