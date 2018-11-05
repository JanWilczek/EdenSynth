#pragma once
/// 
/// \author Jan Wilczek
/// \date 14.10.18
/// 
#include "synth/envelope/ISegmentGain.h"

namespace eden::synth::envelope
{
	/// <summary>
	/// Represents linear gain curve - i.e. logarithmic to the human ear.
	/// </summary>
	class LinearGain : public ISegmentGain
	{
	public:
		virtual ~LinearGain() = default;

		void calculateGain(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel) override;
		void applyAndUpdateGain(SampleType& currentLevel) override;

	private:
		SampleType _gainPerSample = SampleType(0);
	};
}
