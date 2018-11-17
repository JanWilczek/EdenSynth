/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/Decay.h"
#include "utility/EdenAssert.h"
#include "utility/MathConstants.h"
#include <algorithm>

namespace eden::synth::envelope
{
	Decay::Decay(double sampleRate, std::unique_ptr<ISegmentGain> envelopeGain, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
		: EnvelopeSegment(sampleRate, std::move(envelopeGain), duration, initialLevel, finalLevel)
		, _accurateFinalLevel(std::max(finalLevel + 1e-6f, math_constants::THRESHOLD_OF_HEARING))
	{
		EDEN_ASSERT(initialLevel > finalLevel);
	}

	bool Decay::hasEnded(SampleType currentGain)
	{
		return currentGain <= _accurateFinalLevel;
	}
}
