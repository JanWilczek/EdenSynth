/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/Attack.h"
#include "synth/envelope/IEnvelopeGain.h"
#include "utility/EdenAssert.h"

namespace eden::synth::envelope
{
	Attack::Attack(double sampleRate, std::unique_ptr<IEnvelopeGain> envelopeGain, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
		: EnvelopeSegment(sampleRate, std::move(envelopeGain), duration, initialLevel, finalLevel)
	{
		EDEN_ASSERT(initialLevel <= finalLevel);
	}

	bool Attack::hasEnded(SampleType currentGain)
	{
		constexpr auto EPSILON = 2e-4;
		return currentGain >= _finalLevel - EPSILON;
	}
}
