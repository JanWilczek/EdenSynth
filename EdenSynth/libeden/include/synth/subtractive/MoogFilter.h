#pragma once
/// 
/// \author Jan Wilczek
/// \date 25.11.2018
/// 
#include "synth/subtractive/OnePoleLowPassFilter.h"
#include "eden/FilterParameters.h"

namespace eden::synth::subtractive
{
	class MoogFilter
	{
	public:
		MoogFilter(float sampleRate);
		float processSample(float sample);
		void setCutoffFrequency(float cutoffFrequency);
		void setResonance(float resonance);
		void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);
		void setSampleRate(float sampleRate);

	private:
		void calculateGRes();

		float _sampleRate;

		OnePoleLowPassFilter _afterA;
		OnePoleLowPassFilter _afterB;
		OnePoleLowPassFilter _afterC;
		OnePoleLowPassFilter _afterD;

		const float _a = 0.f;
		const float _b = 0.f;
		float _c;
		const float _d = 0.f;
		float _e;

		float _cutoffFrequency;
		float _resonance;

		float _gRes;
		const float _gComp = 0.5f;

		float _feedbackSample;
	};
}
