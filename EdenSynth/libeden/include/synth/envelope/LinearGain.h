#pragma once
/// 
/// \author Jan Wilczek
/// \date 14.10.18
/// 
#include "synth/envelope/IEnvelopeGain.h"

namespace eden::synth::envelope
{
	class LinearGain : public IEnvelopeGain
	{
	public:
		virtual ~LinearGain() = default;

		void calculateGain(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel) override;
		void applyAndUpdateGain(SampleType& currentLevel) override;

	private:
		SampleType _gainPerSample = SampleType(0);
	};
}
