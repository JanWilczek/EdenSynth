///
///	\author Jan Wilczek 
///	 \date 08.10.2018
///
#include <cmath>
#include "synth/subtractive/SubtractiveModule.h"

namespace eden::synth::subtractive
{
	SubtractiveModule::SubtractiveModule(float sampleRate)
		: _filter(sampleRate)
		, _cutoff(0.f)
		, _pitch(0.f)
	{
	}

	void SubtractiveModule::process(float* audioChannel, int startSample, int samplesToProcess)
	{
		for (auto sample = startSample; sample < startSample + samplesToProcess; ++sample)
		{
			audioChannel[sample] = _filter.processSample(audioChannel[sample]);
		}
	}

	void SubtractiveModule::setCutoff(float cutoff)
	{
		_cutoff = cutoff;
		setCutoffFrequency();
	}

	void SubtractiveModule::setResonance(float resonance)
	{
		_filter.setResonance(resonance);
	}

	void SubtractiveModule::setPassbandAttenuation(PassbandAttenuation passbandAttenuation)
	{
		_filter.setPassbandAttenuation(passbandAttenuation);
	}

	void SubtractiveModule::setSampleRate(float sampleRate)
	{
		_filter.setSampleRate(sampleRate);
	}

	void SubtractiveModule::setPitch(float pitch)
	{
		_pitch = pitch;
		setCutoffFrequency();
	}

	void SubtractiveModule::setCutoffFrequency()
	{
		const auto newCutoffFrequency = _pitch * _cutoff;
		_filter.setCutoffFrequency(newCutoffFrequency);
	}
}
