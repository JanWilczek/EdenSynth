///
/// \author Jan Wilczek
/// \date 04.12.2018
///
#include "WaveshapingTransferFunctionContainer.h"
#include "eden/EdenSynthesiser.h"

namespace eden_vst {
WaveshapingTransferFunctionContainer::WaveshapingTransferFunctionContainer(
    std::vector<float> transferFunction)
    : _transferFunction(std::move(transferFunction)) {}

void WaveshapingTransferFunctionContainer::setTransferFunction(
    std::vector<float> transferFunction) {
  _transferFunction = transferFunction;
  _hasChanged.store(true);
}

std::vector<float> WaveshapingTransferFunctionContainer::getTransferFunction()
    const {
  return _transferFunction;
}

void WaveshapingTransferFunctionContainer::passTransferFunction(
    eden::EdenSynthesiser& synthesiser) {
  bool compare = true;
  while (!_hasChanged.compare_exchange_weak(compare, false)) {
    synthesiser.setWaveshapingTransferFunction(_transferFunction);
  }
}
}  // namespace eden_vst
