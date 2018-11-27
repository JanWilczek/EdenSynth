#pragma once
/// 
/// \author Jan Wilczek
/// \date 27.11.2018
/// 
#include "synth/envelope/Envelope.h"
#include "synth/envelope/Attack.h"
#include "synth/envelope/Decay.h"
#include "synth/envelope/Sustain.h"

namespace eden
{
	struct ADSRParameters;
}

namespace eden::synth::envelope
{
	class ADSR : public Envelope
	{
	public:
		enum class ADSRSegments
		{
			Attack = 0,
			Decay = 1,
			Sustain = 2,
			Release = 3,
			Silence = 4
		};

		ADSR(float sampleRate, ADSRParameters parameters);
		~ADSR() override = default;

		void keyOn() override;
		void keyOff() override;

	protected:
		bool hasEnded() override;

	private:
		Attack _attack;
		Decay _decay;
		Sustain _sustain;
		Decay _release;
		Sustain _silence;
	};
}
