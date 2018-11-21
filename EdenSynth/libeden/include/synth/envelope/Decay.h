#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	/// <summary>
	/// Represents decay - time segment when volume should decrease.
	/// </summary>
	class Decay : public EnvelopeSegment
	{
	public:
		Decay(double sampleRate, std::unique_ptr<ISegmentGain> envelopeGain, std::chrono::milliseconds duration, float initialLevel, float finalLevel);
		~Decay() override = default;

		bool hasEnded(float currentLevel) override;

	private:
		/// <summary>
		/// Value to check in hasEnded - can be a little more than passed finalLevel, otherwise decay could never end (e.g. when the exponential gain is used).
		/// </summary>
		float _accurateFinalLevel;
	};
}
