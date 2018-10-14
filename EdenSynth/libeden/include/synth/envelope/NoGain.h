#pragma once
/// 
/// \author Jan Wilczek
/// \date 14.10.18
/// 
#include "synth/envelope/IEnvelopeGain.h"

namespace eden::synth::envelope
{
	class NoGain : public IEnvelopeGain
	{
	public:
		~NoGain() = default;

		void calculateGain(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel) override;
		void applyAndUpdateGain(SampleType& currentGain) override;
	};
}
