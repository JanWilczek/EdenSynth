///
/// \author Jan Wilczek
/// \date 04.12.2018
///
#include "EdenWaveshapingParameters.h"
#include <eden/EdenSynthesiser.h>
#include <limits>
#include "WaveshapingTransferFunctionContainer.h"
#include "eden/WaveshapingFunctionGenerator.h"
#include "ParameterIds.h"

namespace eden::plugin {
EdenWaveshapingParameters::EdenWaveshapingParameters(
    eden::EdenSynthesiser& synthesiser,
    juce::AudioProcessorValueTreeState& apvts)
    : _synthesiser(synthesiser),
      _transferFunction(std::make_shared<WaveshapingTransferFunctionContainer>(
          eden::WaveshapingFunctionGenerator::generateIdentity(400))),
      _waveshaperCurve{apvts,
                       eden::plugin::parameter_id::waveshaperCurveParameter,
                       "Waveshaper curve",
                       juce::StringArray{"Identity", "Hyperbolic tangent",
                                         "Chebyshev polynomial"},
                       0} {}

void EdenWaveshapingParameters::addWaveshapingParameters(
    AudioProcessorValueTreeState& valueTreeState) {
  using Parameter = juce::AudioProcessorValueTreeState::Parameter;

  const std::string autoMakeUpGainParameterName = "waveshaper.autoMakeUpGain";
  valueTreeState.createAndAddParameter(std::make_unique<Parameter>(
      autoMakeUpGainParameterName, "Auto make up gain",
      NormalisableRange<float>(0.f, 1.f, 1.f), 0.f));
  _autoMakeUpGain =
      valueTreeState.getRawParameterValue(autoMakeUpGainParameterName);

  using namespace eden::plugin;
  valueTreeState.createAndAddParameter(std::make_unique<AudioParameterInt>(
      parameter_id::waveshaperChebyshevPolynomialOrderParameter,
      "Chebyshev polynomial order", 2, 10, 2));
  valueTreeState.createAndAddParameter(std::make_unique<AudioParameterInt>(
      parameter_id::waveshaperSpreadSeedParameter, "Waveshaper spread seed", 0,
      std::numeric_limits<int32_t>::max(), 0));
  valueTreeState.createAndAddParameter(std::make_unique<AudioParameterFloat>(
      parameter_id::waveshaperSpreadValueParameter, "Waveshaper spread value",
      juce::NormalisableRange{0.f, 1.f, 0.0001f}, 0.f));

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
}  // namespace eden::plugin
