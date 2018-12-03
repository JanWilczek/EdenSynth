/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/Attack.h"
#include "synth/envelope/ISegmentGain.h"
#include "utility/EdenAssert.h"

namespace eden::synth::envelope
{
	Attack::Attack(float sampleRate, std::unique_ptr<ISegmentGain> envelopeGain, std::chrono::milliseconds duration, float initialLevel, float finalLevel)
		: EnvelopeSegment(sampleRate, std::move(envelopeGain), duration, initialLevel, finalLevel)
	{
		EDEN_ASSERT(initialLevel <= finalLevel);
	}

	bool Attack::hasEnded(float currentGain)
	{
		constexpr auto EPSILON = 2e-4;
		return currentGain >= _finalLevel - EPSILON;
	}
}
