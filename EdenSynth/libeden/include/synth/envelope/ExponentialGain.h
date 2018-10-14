#pragma once
/// 
/// \author Jan Wilczek
/// \date 14.10.18
/// 
#include "synth/envelope/IEnvelopeGain.h"

namespace eden::synth::envelope
{
	class ExponentialGain : public IEnvelopeGain
	{
	public:
		~ExponentialGain() = default;

		void calculateGain(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel) override;
		void applyAndUpdateGain(SampleType& currentGain) override;

	private:
		SampleType _multiplier = SampleType(1);
		const SampleType MINIMUM_LEVEL = SampleType(1e-6);
	};
}
