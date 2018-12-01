/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/SubtractiveModuleSettings.h"
#include "synth/subtractive/SubtractiveModule.h"

namespace eden::settings
{
	SubtractiveModuleSettings::SubtractiveModuleSettings(float sampleRate)
		: _filterEnvelopeSettings(std::make_unique<EnvelopeSettings>(sampleRate))
	{
	}

	void SubtractiveModuleSettings::registerSubtractiveModule(std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule)
	{
		_subtractiveModules.push_back(subtractiveModule);
		_filterEnvelopeSettings->registerEnvelope(subtractiveModule);

		subtractiveModule->setCutoff(_cutoff);
		subtractiveModule->setResonance(_resonance);
		subtractiveModule->setPassbandAttenuation(_passbandAttenuation);
		subtractiveModule->setContourAmount(_contourAmount);
	}

	void SubtractiveModuleSettings::setSampleRate(float sampleRate)
	{
		for (auto subtractiveModule : _subtractiveModules)
		{
			subtractiveModule->setSampleRate(sampleRate);
		}
		_filterEnvelopeSettings->setSampleRate(sampleRate);
	}

	void SubtractiveModuleSettings::setCutoff(float cutoff)
	{
		if (cutoff != _cutoff)
		{
			_cutoff = cutoff;
			for (auto subtractiveModule : _subtractiveModules)
			{
				subtractiveModule->setCutoff(_cutoff);
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
				subtractiveModule->setResonance(_resonance);
			}
		}
	}

	void SubtractiveModuleSettings::setContourAmount(float contourAmount)
	{
		if (contourAmount != _contourAmount)
		{
			_contourAmount = contourAmount;
			for (auto subtractiveModule : _subtractiveModules)
			{
				subtractiveModule->setContourAmount(_contourAmount);
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

	void SubtractiveModuleSettings::setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> parameters)
	{
		_filterEnvelopeSettings->setEnvelopeParameters(parameters);
	}
}
