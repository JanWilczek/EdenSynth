#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/Envelope.h"
#include "synth/envelope/Attack.h"
#include "synth/envelope/Decay.h"
#include "Sustain.h"

namespace eden::synth::envelope
{
	class ADBDR : public Envelope
	{
		using ms = std::chrono::milliseconds;

	public:
		ADBDR(double sampleRate, ms attackTime, ms decay1Time, ms decay2Time, ms releaseTime, SampleType breakLevel = 0.5);
		~ADBDR() override = default;

		void keyOn() override;
		void keyOff() override;

	protected:
		void checkForEnd(unsigned currentSampleIndex) override;

	private:
		/// <summary>
		/// Value of envelope at which segments change from Decay 1 to Decay 2.
		/// </summary>
		SampleType _breakLevel;

		/// <summary>
		/// ADBDR envelope's segments.
		/// </summary>
		Attack _attack;
		Decay _decay1;
		Decay _decay2;
		Decay _release;
		Sustain _silence;
	};
}
