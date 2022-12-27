///
/// \author Jan Wilczek
/// \date 04.12.2018
///
#include "EdenWaveshapingParameters.h"
#include <eden/EdenSynthesiser.h>
#include "WaveshapingTransferFunctionContainer.h"
#include "eden/WaveshapingFunctionGenerator.h"

namespace eden_vst {
EdenWaveshapingParameters::EdenWaveshapingParameters(
    eden::EdenSynthesiser& synthesiser)
    : _synthesiser(synthesiser),
      _transferFunction(std::make_shared<WaveshapingTransferFunctionContainer>(
          eden::WaveshapingFunctionGenerator::generateIdentity(400))) {}

void EdenWaveshapingParameters::addWaveshapingParameters(
    AudioProcessorValueTreeState& valueTreeState) {
  const std::string autoMakeUpGainParameterName = "waveshaper.autoMakeUpGain";
  valueTreeState.createAndAddParameter(
      autoMakeUpGainParameterName, "Auto make up gain", String(),
      NormalisableRange<float>(0.f, 1.f, 1.f), 0.f, nullptr, nullptr);
  _autoMakeUpGain =
      valueTreeState.getRawParameterValue(autoMakeUpGainParameterName);
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
