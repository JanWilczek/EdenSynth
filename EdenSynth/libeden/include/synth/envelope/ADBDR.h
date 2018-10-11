#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/Envelope.h"
#include "synth/envelope/Attack.h"
#include "synth/envelope/Decay.h"
#include "synth/envelope/Release.h"

namespace eden::synth::envelope
{
	class ADBDR : public Envelope
	{
	public:

	private:
		Attack _attack;
		Decay _decay1;
		Decay _decay2;
		Release _release;
		SampleType _breakLevel;
	};
}
