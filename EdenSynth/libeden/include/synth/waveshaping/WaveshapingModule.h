#pragma once
/// 
/// \auhtor Jan Wilczek
/// \date 08.10.2018
/// 
#include "synth/interfaces/IModule.h"

namespace eden::synth::waveshaping
{
	class WaveshapingModule : public IModule
	{
	public:
		~WaveshapingModule() override;

		void process(eden::AudioBuffer& audioBuffer, int startSample, int samplesToProcess) override;
	};
}
