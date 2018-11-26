/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/SubtractiveModuleSettings.h"
#include "synth/subtractive/SubtractiveModule.h"

namespace eden::settings
{
	void SubtractiveModuleSettings::registerSubtractiveModule(std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule)
	{
		_subtractiveModules.push_back(subtractiveModule);

		subtractiveModule->setCutoff(_cutoff);
		subtractiveModule->setResonance(_resonance);
		subtractiveModule->setPassbandAttenuation(_passbandAttenuation);
	}

	void SubtractiveModuleSettings::setSampleRate(float sampleRate)
	{
		for (auto subtractiveModule : _subtractiveModules)
		{
			subtractiveModule->setSampleRate(sampleRate);
		}
	}

	void SubtractiveModuleSettings::setCutoff(float cutoff)
	{
		if (cutoff != _cutoff)
		{
			_cutoff = cutoff;
			for (auto subtractiveModule : _subtractiveModules)
			{
				subtractiveModule->setCutoff(cutoff);
			}
		}
	}

	void SubtractiveModuleSettings::setResonance(float resonance)
	{
		if (resonance != _resonance)
		{
			_resonance = resonance;
			for (auto subtractiveModule : _subtractiveModules)
			{
				subtractiveModule->setResonance(resonance);
			}
		}
	}

	void SubtractiveModuleSettings::setPassbandAttenuation(PassbandAttenuation passbandAttenuation)
	{
		if (passbandAttenuation != _passbandAttenuation)
		{
			_passbandAttenuation = passbandAttenuation;
			for (auto subtractiveModule : _subtractiveModules)
			{
				subtractiveModule->setPassbandAttenuation(_passbandAttenuation);
			}
		}
	}
}
