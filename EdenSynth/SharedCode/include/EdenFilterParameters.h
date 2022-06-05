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

		std::atomic<float>* _cutoff = nullptr;
		std::atomic<float>* _resonance = nullptr;
		std::atomic<float>* _contourAmount = nullptr;
		std::atomic<float>* _passbandAttenuation = nullptr;

		std::atomic<float>* _attackTime = nullptr;
		std::atomic<float>* _decayTime = nullptr;
		std::atomic<float>* _sustainLevel = nullptr;
		std::atomic<float>* _releaseTime = nullptr;
	};
}
