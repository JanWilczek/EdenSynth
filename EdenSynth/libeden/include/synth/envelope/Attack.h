#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	class Attack : public EnvelopeSegment
	{
	public:
		Attack(double sampleRate, std::unique_ptr<IEnvelopeGain> envelopeGain, std::chrono::milliseconds duration, SampleType initialLevel = 0, SampleType finalLevel = 1.0);
		~Attack() override = default;

		bool hasEnded(SampleType currentLevel) override;
	};
}