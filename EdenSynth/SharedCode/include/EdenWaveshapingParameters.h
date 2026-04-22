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

namespace eden::plugin {
class WaveshapingTransferFunctionContainer;

class EdenWaveshapingParameters {
public:
  EdenWaveshapingParameters(eden::EdenSynthesiser&,
                            juce::AudioProcessorValueTreeState&);

  void updateWaveshapingParameters();
  std::shared_ptr<WaveshapingTransferFunctionContainer>
  getTransferFunctionContainer() const noexcept;

private:
  eden::EdenSynthesiser& _synthesiser;
  std::shared_ptr<WaveshapingTransferFunctionContainer> _transferFunction;

  eden::plugin::util::ParameterWrapper<juce::AudioParameterChoice>
      _waveshaperCurve;
};
}  // namespace eden::plugin
