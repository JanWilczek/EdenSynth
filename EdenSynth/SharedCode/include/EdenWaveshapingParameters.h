#pragma once
///
/// \author Jan Wilczek
/// \date 04.12.2018
///
#include <JuceHeader.h>

namespace eden {
class EdenSynthesiser;
}

namespace eden::plugin {
class WaveshapingTransferFunctionContainer;

class EdenWaveshapingParameters {
public:
  explicit EdenWaveshapingParameters(eden::EdenSynthesiser&);

  void updateWaveshapingParameters();
  std::shared_ptr<WaveshapingTransferFunctionContainer>
  getTransferFunctionContainer() const noexcept;

private:
  eden::EdenSynthesiser& _synthesiser;
  std::shared_ptr<WaveshapingTransferFunctionContainer> _transferFunction;
};
}  // namespace eden::plugin
