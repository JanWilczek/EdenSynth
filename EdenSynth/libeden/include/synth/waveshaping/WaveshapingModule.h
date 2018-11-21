#pragma once
/// 
/// \auhtor Jan Wilczek
/// \date 08.10.2018
/// 
#include "synth/IMonoModule.h"

namespace eden::synth::waveshaping
{
	class WaveshapingModule : public IMonoModule
	{
	public:
		~WaveshapingModule() override;

		void process(float* audioChannel, int startSample, int samplesToProcess) override;
	};
}
