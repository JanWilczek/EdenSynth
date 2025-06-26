#pragma once
///
/// \author Jan Wilczek
/// \date 04.12.2018
///
#include <JuceHeader.h>
#include "util/ParameterWrapper.h"

namespace eden {
class EdenSynthesiser;
}

namespace eden_vst {
class WaveshapingTransferFunctionContainer;

class EdenWaveshapingParameters {
public:
  EdenWaveshapingParameters(eden::EdenSynthesiser&,
                            juce::AudioProcessorValueTreeState&);

  void addWaveshapingParameters(AudioProcessorValueTreeState&);
  void updateWaveshapingParameters();
  std::shared_ptr<WaveshapingTransferFunctionContainer>
  getTransferFunctionContainer() const noexcept;

private:
  eden::EdenSynthesiser& _synthesiser;
  std::atomic<float>* _autoMakeUpGain = nullptr;
  std::shared_ptr<WaveshapingTransferFunctionContainer> _transferFunction;

  eden::plugin::util::ParameterWrapper<juce::AudioParameterChoice>
      _waveshaperCurve;
};
}  // namespace eden_vst
