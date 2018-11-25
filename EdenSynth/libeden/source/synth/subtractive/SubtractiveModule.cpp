///
///	\author Jan Wilczek 
///	 \date 08.10.2018
///
#include "synth/subtractive/SubtractiveModule.h"

namespace eden::synth::subtractive
{
	SubtractiveModule::SubtractiveModule(float sampleRate)
		: _filter(sampleRate)
	{
	}

	void SubtractiveModule::process(float* audioChannel, int startSample, int samplesToProcess)
	{
		for (auto sample = startSample; sample < startSample + samplesToProcess; ++sample)
		{
			audioChannel[sample] = _filter.processSample(audioChannel[sample]);
		}
	}

	void SubtractiveModule::setCutoffFrequency(float cutoffFrequency)
	{
		_filter.setCutoffFrequency(cutoffFrequency);
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

	void SubtractiveModule::setPitch(double pitch)
	{
		// TODO: No pitch follow implemented yet.
	}

	void SubtractiveModule::setPitchBend(int pitchBendInSemitones)
	{
		// TODO: No pitch follow implemented yet.
	}
}
