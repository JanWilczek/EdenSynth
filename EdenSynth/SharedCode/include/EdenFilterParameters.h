#pragma once
/// 
/// \author Jan Wilczek
/// \date 01.12.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

namespace eden 
{
	class EdenSynthesiser;
}

namespace eden_vst
{
	class EdenFilterParameters
	{
	public:
		explicit EdenFilterParameters(eden::EdenSynthesiser& synthesiser);
		void addFilterParameters(AudioProcessorValueTreeState& pluginParameters);
		void updateFilterParameters();

	private:
		eden::EdenSynthesiser& _synthesiser;

		float* _cutoff = nullptr;
		float* _resonance = nullptr;
		float* _contourAmount = nullptr;
		float* _passbandAttenuation = nullptr;

		float* _attackTime = nullptr;
		float* _decayTime = nullptr;
		float* _sustainLevel = nullptr;
		float* _releaseTime = nullptr;
	};
}
