#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	class Sustain : public EnvelopeSegment
	{
	public:
		explicit Sustain();
		~Sustain() override = default;

		bool hasEnded(SampleType currentLevel) override;
		void reset() override;
	};
}
