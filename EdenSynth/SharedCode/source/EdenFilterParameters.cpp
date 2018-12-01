/// 
/// \author Jan Wilczek
/// \date 01.12.2018
/// 
#include "EdenFilterParameters.h"
#include <eden/EdenSynthesiser.h>

namespace eden_vst
{
	EdenFilterParameters::EdenFilterParameters(eden::EdenSynthesiser& synthesiser)
		: _synthesiser(synthesiser)
	{
	}

	void EdenFilterParameters::addFilterParameters(AudioProcessorValueTreeState& pluginParameters)
	{
		const std::string cutoffParameterName = "filter.cutoff";
		pluginParameters.createAndAddParameter(cutoffParameterName, "Cutoff", String(), NormalisableRange<float>(0.1f, 75.f, 0.001f, 0.3f), 1.f, nullptr, nullptr);
		_cutoff = pluginParameters.getRawParameterValue(cutoffParameterName);

		const std::string resonanceParameterName = "filter.resonance";
		pluginParameters.createAndAddParameter(resonanceParameterName, "Resonance", String(), NormalisableRange<float>(0.f, 0.9f, 0.0001f), 0.f, nullptr, nullptr);
		_resonance = pluginParameters.getRawParameterValue(resonanceParameterName);

		const std::string contourAmountParameterName = "filter.contourAmount";
		pluginParameters.createAndAddParameter(contourAmountParameterName, "Contour amount", String(), NormalisableRange<float>(0.f, 1.0f, 0.001f, 0.4f), 1.0f, nullptr, nullptr);
		_contourAmount = pluginParameters.getRawParameterValue(contourAmountParameterName);

		const std::string passbandAttenuationParameterName = "filter.passbandAttenuation";
		pluginParameters.createAndAddParameter(passbandAttenuationParameterName, "Passband attenuation", String(), NormalisableRange(0.f, 1.f, 1.f), 0.f, nullptr, nullptr);
		_passbandAttenuation = pluginParameters.getRawParameterValue(passbandAttenuationParameterName);

		const std::string attackTimeParameterName = "filter.envelope.attackTime";
		pluginParameters.createAndAddParameter(attackTimeParameterName, "Filter attack time", "ms", NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f), 50.f, nullptr, nullptr);
		_attackTime = pluginParameters.getRawParameterValue(attackTimeParameterName);

		const std::string decayTimeParameterName = "filter.envelope.decayTime";
		pluginParameters.createAndAddParameter(decayTimeParameterName, "Filter decay time", "ms", NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f), 20.f, nullptr, nullptr);
		_decayTime = pluginParameters.getRawParameterValue(decayTimeParameterName);

		const std::string sustainLevelParameterName = "filter.envelope.sustainLevel";
		pluginParameters.createAndAddParameter(sustainLevelParameterName, "Filter sustain level", String(), NormalisableRange<float>(0.f, 1.f, 0.001f, 0.4f), 0.9f, nullptr, nullptr);
		_sustainLevel = pluginParameters.getRawParameterValue(sustainLevelParameterName);
		
		const std::string releaseTimeParameterName = "filter.envelope.releaseTime";
		pluginParameters.createAndAddParameter(releaseTimeParameterName, "Filter release time", "ms", NormalisableRange<float>(1.f, 40000.f, 1.f, 0.3f), 300.f, nullptr, nullptr);
		_releaseTime = pluginParameters.getRawParameterValue(releaseTimeParameterName);
	}

	void EdenFilterParameters::updateFilterParameters(const AudioProcessorValueTreeState& pluginParameters)
	{
		_synthesiser.setCutoff(*_cutoff);
		_synthesiser.setResonance(*_resonance);
		_synthesiser.setContourAmount(*_contourAmount);
		_synthesiser.setPassbandAttenuation(static_cast<eden::PassbandAttenuation>(static_cast<int>(*_passbandAttenuation)));

		std::shared_ptr<eden::ADSRParameters> filterEnvelopeParameters = std::make_shared<eden::ADSRParameters>();
		filterEnvelopeParameters->attackTime = std::chrono::milliseconds(static_cast<int>(*_attackTime));
		filterEnvelopeParameters->decayTime = std::chrono::milliseconds(static_cast<int>(*_decayTime));
		filterEnvelopeParameters->sustainLevel = *_sustainLevel;
		filterEnvelopeParameters->releaseTime = std::chrono::milliseconds(static_cast<int>(*_releaseTime));

		_synthesiser.setFilterEnvelopeParameters(filterEnvelopeParameters);
	}
}
