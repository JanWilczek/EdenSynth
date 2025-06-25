///
/// \author Jan Wilczek
/// \date 04.12.2018
///
#include "EdenWaveshapingParameters.h"
#include <eden/EdenSynthesiser.h>
#include "WaveshapingTransferFunctionContainer.h"
#include "eden/WaveshapingFunctionGenerator.h"
#include "ParameterIds.h"

namespace eden_vst {
EdenWaveshapingParameters::EdenWaveshapingParameters(
    eden::EdenSynthesiser& synthesiser)
    : _synthesiser(synthesiser),
      _transferFunction(std::make_shared<WaveshapingTransferFunctionContainer>(
          eden::WaveshapingFunctionGenerator::generateIdentity(400))) {}

void EdenWaveshapingParameters::addWaveshapingParameters(
    AudioProcessorValueTreeState& valueTreeState) {
  using Parameter = juce::AudioProcessorValueTreeState::Parameter;

  const std::string autoMakeUpGainParameterName = "waveshaper.autoMakeUpGain";
  valueTreeState.createAndAddParameter(std::make_unique<Parameter>(
      autoMakeUpGainParameterName, "Auto make up gain",
      NormalisableRange<float>(0.f, 1.f, 1.f), 0.f));
  _autoMakeUpGain =
      valueTreeState.getRawParameterValue(autoMakeUpGainParameterName);

  // TODO: Add
  // - spread value
  // - spread seed
  using namespace eden::plugin;
  valueTreeState.createAndAddParameter(std::make_unique<AudioParameterChoice>(
      parameter_id::WAVESHAPER_CURVE_PARAMETER, "Waveshaper curve",
      StringArray{"Identity", "Hyperbolic tangent", "Chebyshev polynomial"},
      0));
  valueTreeState.createAndAddParameter(std::make_unique<AudioParameterInt>(
      parameter_id::WAVESHAPER_CHEBYSHEV_POLYNOMIAL_ORDER_PARAMETER,
      "Chebyshev polynomial order", 2, 1000, 2));

  // TODO: Set the transfer function based on parameter values (incl.
  // setStateInformation()).
}

void EdenWaveshapingParameters::updateWaveshapingParameters() {
  _synthesiser.setWaveshapingAutoMakeUpGain(
      static_cast<bool>(*_autoMakeUpGain));

  _transferFunction->passTransferFunction(_synthesiser);
}

std::shared_ptr<WaveshapingTransferFunctionContainer>
EdenWaveshapingParameters::getTransferFunctionContainer() const noexcept {
  return _transferFunction;
}
}  // namespace eden_vst
