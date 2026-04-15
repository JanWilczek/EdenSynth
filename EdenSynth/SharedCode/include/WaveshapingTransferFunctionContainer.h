#pragma once
///
/// \author Jan Wilczek
/// \date 04.12.2018
///
#include <atomic>
#include <vector>

namespace eden {
class EdenSynthesiser;
}

namespace eden::plugin {
class WaveshapingTransferFunctionContainer {
public:
  WaveshapingTransferFunctionContainer(
      std::vector<float> transferFunction);  // TODO: Is initializing transfer
                                             // function really necessary?

  void setTransferFunction(std::vector<float> transferFunction);
  std::vector<float> getTransferFunction() const;
  void passTransferFunction(eden::EdenSynthesiser& synthesiser);

private:
  std::atomic<bool> _hasChanged{false};
  std::vector<float> _transferFunction;
};
}  // namespace eden::plugin
