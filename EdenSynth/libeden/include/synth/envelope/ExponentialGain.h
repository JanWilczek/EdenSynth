#pragma once
/// 
/// \author Jan Wilczek
/// \date 14.10.18
/// 
#include "synth/envelope/IEnvelopeGain.h"
#include <limits>

namespace eden::synth::envelope
{
	/// <summary>
	/// Represents exponential gain curve - i.e. linear to the human ear.
	/// </summary>
	class ExponentialGain : public IEnvelopeGain
	{
	public:
		~ExponentialGain() = default;

		void calculateGain(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel) override;
		void applyAndUpdateGain(SampleType& currentGain) override;

	private:
		SampleType _multiplier = SampleType(1);
		const SampleType MINIMUM_LEVEL = SampleType(std::numeric_limits<float>::epsilon());
	};
}
