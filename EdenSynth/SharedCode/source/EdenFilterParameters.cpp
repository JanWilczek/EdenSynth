///
/// \author Jan Wilczek
/// \date 01.12.2018
///
#include "EdenFilterParameters.h"
#include <eden/EdenSynthesiser.h>

namespace eden_vst {
EdenFilterParameters::EdenFilterParameters(eden::EdenSynthesiser& synthesiser)
    : _synthesiser(synthesiser) {}

void EdenFilterParameters::addFilterParameters(
    AudioProcessorValueTreeState& pluginParameters) {
  using Parameter = juce::AudioProcessorValueTreeState::Parameter;

  const std::string cutoffParameterName = "filter.cutoff";
  pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
      cutoffParameterName, "Cutoff",
      NormalisableRange<float>(0.1f, 75.f, 0.001f, 0.3f), 1.f));
  _cutoff = pluginParameters.getRawParameterValue(cutoffParameterName);

  const std::string resonanceParameterName = "filter.resonance";
  pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
      resonanceParameterName, "Resonance",
      NormalisableRange<float>(0.f, 1.f, 0.0001f), 0.f));
  _resonance = pluginParameters.getRawParameterValue(resonanceParameterName);

  const std::string contourAmountParameterName = "filter.contourAmount";
  pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
      contourAmountParameterName, "Contour amount",
      NormalisableRange<float>(0.f, 1.0f, 0.001f, 1.6f), 1.0f));
  _contourAmount =
      pluginParameters.getRawParameterValue(contourAmountParameterName);

  const std::string passbandAttenuationParameterName =
      "filter.passbandAttenuation";
  pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
      passbandAttenuationParameterName, "Passband attenuation",
      NormalisableRange(0.f, 1.f, 1.f), 0.f));
  _passbandAttenuation =
      pluginParameters.getRawParameterValue(passbandAttenuationParameterName);

  const std::string attackTimeParameterName =
      "filter.env.adsr.attack.time";
  pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
      attackTimeParameterName, "Filter attack time",
      NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f), 50.f,
      AudioProcessorValueTreeStateParameterAttributes{}.withLabel("ms")));
  _attackTime = pluginParameters.getRawParameterValue(attackTimeParameterName);

  const std::string decayTimeParameterName = "filter.env.adsr.decay.time";
  pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
      decayTimeParameterName, "Filter decay time",
      NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f), 20.f,
      AudioProcessorValueTreeStateParameterAttributes{}.withLabel("ms")));
  _decayTime = pluginParameters.getRawParameterValue(decayTimeParameterName);

  const std::string sustainLevelParameterName =
      "filter.env.adsr.sustain.level";
  pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
      sustainLevelParameterName, "Filter sustain level",
      NormalisableRange<float>(0.f, 1.f, 0.001f, 0.4f), 0.9f));
  _sustainLevel =
      pluginParameters.getRawParameterValue(sustainLevelParameterName);

  const std::string releaseTimeParameterName =
      "filter.env.adsr.release.time";
  pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
      releaseTimeParameterName, "Filter release time",
      NormalisableRange<float>(1.f, 40000.f, 1.f, 0.3f), 300.f,
      AudioProcessorValueTreeStateParameterAttributes{}.withLabel("ms")));
  _releaseTime =
      pluginParameters.getRawParameterValue(releaseTimeParameterName);
}

void EdenFilterParameters::updateFilterParameters() {
  _synthesiser.setCutoff(*_cutoff);
  _synthesiser.setResonance(*_resonance);
  _synthesiser.setContourAmount(*_contourAmount);
  _synthesiser.setPassbandAttenuation(static_cast<eden::PassbandAttenuation>(
      static_cast<int>(*_passbandAttenuation)));

  std::shared_ptr<eden::ADSRParameters> filterEnvelopeParameters =
      std::make_shared<eden::ADSRParameters>();
  filterEnvelopeParameters->attackTime =
      std::chrono::milliseconds(static_cast<int>(*_attackTime));
  filterEnvelopeParameters->decayTime =
      std::chrono::milliseconds(static_cast<int>(*_decayTime));
  filterEnvelopeParameters->sustainLevel = *_sustainLevel;
  filterEnvelopeParameters->releaseTime =
      std::chrono::milliseconds(static_cast<int>(*_releaseTime));

  _synthesiser.setFilterEnvelopeParameters(filterEnvelopeParameters);
}
}  // namespace eden_vst
