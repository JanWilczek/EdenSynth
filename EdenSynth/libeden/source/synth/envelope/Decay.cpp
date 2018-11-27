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
	Decay::Decay(double sampleRate, std::unique_ptr<ISegmentGain> envelopeGain, std::chrono::milliseconds duration, float initialLevel, float finalLevel)
		: EnvelopeSegment(sampleRate, std::move(envelopeGain), duration, initialLevel, finalLevel)
	{
		EDEN_ASSERT(initialLevel > finalLevel);
	}

	bool Decay::hasEnded(float currentGain)
	{
		// Value to check in hasEnded - can be a little more than passed finalLevel, otherwise decay could never end (e.g. when the exponential gain is used).
		const auto accurateFinalLevel = std::max(_finalLevel + 1e-6f, math_constants::THRESHOLD_OF_HEARING);
		return currentGain <= accurateFinalLevel;
	}
}
