#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>
#include "eden/FilterParameters.h"

namespace eden::synth::subtractive
{
	class SubtractiveModule;
}

namespace eden::settings
{
	class SubtractiveModuleSettings
	{
	public:
		void registerSubtractiveModule(std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule);

		void setSampleRate(float sampleRate);
		void setCutoff(float cutoff);
		void setResonance(float resonance);
		void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);

	private:
		std::vector<std::shared_ptr<synth::subtractive::SubtractiveModule>> _subtractiveModules;
		float _cutoff = 20000.f;
		float _resonance = 0.f;
		PassbandAttenuation _passbandAttenuation = PassbandAttenuation::dB12;
	};
}
