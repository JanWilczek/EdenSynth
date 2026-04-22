///
/// \author Jan Wilczek
/// \date 04.12.2018
///
#include "EdenWaveshapingParameters.h"
#include <eden/EdenSynthesiser.h>
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

void EdenWaveshapingParameters::updateWaveshapingParameters() {
  _transferFunction->passTransferFunction(_synthesiser);
}

std::shared_ptr<WaveshapingTransferFunctionContainer>
EdenWaveshapingParameters::getTransferFunctionContainer() const noexcept {
  return _transferFunction;
}
}  // namespace eden::plugin
