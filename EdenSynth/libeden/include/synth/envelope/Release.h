#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	class Release : public EnvelopeSegment
	{
	public:
		Release(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel = 0.0);
		~Release() override = default;

		bool hasEnded(SampleType currentGain) override;
		void reset() override;
	};
}
