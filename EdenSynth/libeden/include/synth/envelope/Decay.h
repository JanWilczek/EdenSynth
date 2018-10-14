#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	class Decay : public EnvelopeSegment
	{
	public:
		Decay(double sampleRate, std::unique_ptr<IEnvelopeGain> envelopeGain, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel);
		~Decay() override = default;

		bool hasEnded(SampleType currentLevel) override;
		void reset() override;
	};
}
