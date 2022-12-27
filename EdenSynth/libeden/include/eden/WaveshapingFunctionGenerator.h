#pragma once
///
/// \author Jan Wilczek
/// \date 03.12.2018
///
#include <functional>
#include <vector>

namespace eden {
class WaveshapingFunctionGenerator {
 public:
  static std::vector<float> generateIdentity(unsigned length);

  static std::vector<float> generateTransferFunction(
      std::function<float(float)> generator,
      unsigned int length = 48000);

  static std::vector<float> generateChebyshevPolynomial(
      unsigned int order,
      unsigned int length = 48000);

  static void spreadValuesRandomly(std::vector<float>& function,
                                   float spreadRange);
};
}  // namespace eden
