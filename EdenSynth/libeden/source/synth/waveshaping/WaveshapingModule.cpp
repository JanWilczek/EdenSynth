/// 
/// \auhtor Jan Wilczek
/// \date 08.10.2018
/// 
#include "synth/waveshaping/WaveshapingModule.h"

namespace eden::synth::waveshaping
{
	void WaveshapingModule::process(float* audioChannel, int startSample, int samplesToProcess)
	{
		for (auto sample = startSample; sample < startSample + samplesToProcess; ++sample)
		{
			audioChannel[sample] = _waveshaper.transform(audioChannel[sample]);
		}
	}

	void WaveshapingModule::setTransferFunction(std::vector<float> transferFunction)
	{
		_waveshaper.setTransferFunction(std::move(transferFunction));
	}

	void WaveshapingModule::setAutoMakeUpGain(bool makeUpGainEnabled)
	{
		_waveshaper.setAutoMakeUpGain(makeUpGainEnabled);
	}
}
