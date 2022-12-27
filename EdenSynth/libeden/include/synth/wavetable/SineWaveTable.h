#pragma once
///
/// \author Jan Wilczek
/// \date 20.10.2018
///
#include <cmath>
#include <vector>
#include "utility/MathConstants.h"

namespace eden::synth::wavetable {
const std::vector<float> SineWaveTable = []() {
  constexpr size_t size = 48000u;
  std::vector<float> result(size);
  for (size_t i = 0; i < size; ++i) {
    result[i] = static_cast<float>(std::sin(2 * math_constants::PI * i / size));
  }

  return result;
}();
}
