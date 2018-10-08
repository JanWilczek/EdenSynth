#pragma once
///
///	\author Jan Wilczek 
///	 \date 08.10.2018
///
#include "synth/interfaces/IModule.h"

namespace eden::synth::subtractive
{
	class SubtractiveModule : public IModule
	{
	public:
		~SubtractiveModule() override;

		void process(eden::AudioBuffer& audioBuffer, int startSample, int samplesToProcess) override;
	};
}
