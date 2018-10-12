#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/Envelope.h"
#include "synth/envelope/Attack.h"
#include "synth/envelope/Decay.h"
#include "synth/envelope/Release.h"
#include "Sustain.h"

namespace eden::synth::envelope
{
	class ADBDR : public Envelope
	{
		using ms = std::chrono::milliseconds;

	public:
		ADBDR(double sampleRate, ms attackTime, ms decay1Time, ms decay2Time, ms releaseTime, SampleType breakLevel = 0.5);
		~ADBDR() override = default;

		void keyOff() override;

	protected:
		void checkForEnd() override;

	private:
		SampleType _breakLevel;
		Attack _attack;
		Decay _decay1;
		Decay _decay2;
		Release _release;
		Sustain _silence;
	};
}
