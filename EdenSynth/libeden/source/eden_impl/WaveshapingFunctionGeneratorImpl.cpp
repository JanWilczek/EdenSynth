///
/// \author Jan Wilczek
/// \date 03.12.2018
///
#include "eden_impl/WaveshapingFunctionGeneratorImpl.h"
#include <algorithm>
#include <cstdlib>

namespace eden {
std::vector<float> WaveshapingFunctionGeneratorImpl::generateIdentity(
    const unsigned length) {
  return generateTransferFunction([](const float x) { return x; }, length);
}

std::vector<float> WaveshapingFunctionGeneratorImpl::generateTransferFunction(
    std::function<float(float)> generator,
    unsigned length) {
  std::vector<float> output(length);

  constexpr auto lowerBoundary = -1.f;
  constexpr auto upperBoundary = 1.f;

  const auto dx = (upperBoundary - lowerBoundary) / (length - 1);
  auto x = lowerBoundary;

  for (auto i = 0u; i < length; ++i) {
    output[i] = std::clamp(generator(x), -1.f, 1.f);

    x += dx;
  }

  return output;
}

std::vector<float>
WaveshapingFunctionGeneratorImpl::generateChebyshevPolynomial(
    unsigned order,
    const unsigned length) {
  if (order == 0u) {
    return std::vector<float>(length, 1.f);
  }

  if (order == 1u) {
    return generateIdentity(length);
  }

  auto Tn_minus1 = generateChebyshevPolynomial(0u, length);
  auto Tn = generateChebyshevPolynomial(1u, length);
  auto n = 1u;

  while (n != order) {
    std::vector<float> Tn_plus1(length);

    // Tn+1 = 2 * x * Tn(x) - Tn-1(x)
    for (auto i = 0u; i < length; ++i) {
      auto x = static_cast<float>(2 * i) / (length - 1u) - 1.f;

      Tn_plus1[i] = 2.f * x * Tn[i] - Tn_minus1[i];
    }

    Tn_minus1 = std::move(Tn);
    Tn = std::move(Tn_plus1);
    n++;
  }

  return Tn;
}

void WaveshapingFunctionGeneratorImpl::spreadValuesRandomly(
    std::vector<float>& function,
    float spreadRange) {
  for (auto& value : function) {
    value += 2.f * spreadRange * std::rand() / static_cast<float>(RAND_MAX) -
             spreadRange;
    value = std::clamp(value, -1.f, 1.f);
  }
}
}  // namespace eden
