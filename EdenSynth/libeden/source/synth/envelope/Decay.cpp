/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/Decay.h"
#include "utility/EdenAssert.h"

namespace eden::synth::envelope
{
	Decay::Decay(double sampleRate, std::unique_ptr<ISegmentGain> envelopeGain, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
		: EnvelopeSegment(sampleRate, std::move(envelopeGain), duration, initialLevel, finalLevel)
	{
		EDEN_ASSERT(initialLevel >= finalLevel);
	}

	bool Decay::hasEnded(SampleType currentGain)
	{
		constexpr auto EPSILON = 1e-6;
		return currentGain <= _finalLevel + EPSILON;
	}
}
