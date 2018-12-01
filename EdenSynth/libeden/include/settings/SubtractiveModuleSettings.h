#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>
#include "eden/FilterParameters.h"
#include "eden/EnvelopeParameters.h"
#include "settings/EnvelopeSettings.h"

namespace eden::synth::subtractive
{
	class SubtractiveModule;
}

namespace eden::settings
{
	class SubtractiveModuleSettings
	{
	public:
		SubtractiveModuleSettings(float sampleRate);
		void registerSubtractiveModule(std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule);

		void setSampleRate(float sampleRate);
		void setCutoff(float cutoff);
		void setResonance(float resonance);
		void setContourAmount(float contourAmount);
		void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);
		void setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> parameters);

	private:
		std::vector<std::shared_ptr<synth::subtractive::SubtractiveModule>> _subtractiveModules;
		float _cutoff = 100.f;
		float _resonance = 0.f;
		float _contourAmount = 1.f;
		PassbandAttenuation _passbandAttenuation = PassbandAttenuation::dB12;
		std::unique_ptr<EnvelopeSettings> _filterEnvelopeSettings;
	};
}
