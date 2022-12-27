///
/// \author Jan Wilczek
/// \date 20.10.2018
///
#include "interpolation/LinearInterpolator.h"
#include <cmath>
#include "utility/EdenAssert.h"

namespace eden::interpolation {
LinearInterpolator::~LinearInterpolator() {}

float LinearInterpolator::interpolate(const std::vector<float>& discreteValues,
                                      float index) {
  EDEN_ASSERT(index >= 0 && index < discreteValues.size());

  const auto lowerIndex = static_cast<int>(std::floor(index));
  const auto upperIndex = static_cast<int>(std::ceil(index));

  // if an integer index was given return precise value from the vector
  if (lowerIndex == upperIndex) {
    return discreteValues[lowerIndex];
  }

  const auto toLower = index - lowerIndex;
  const auto toUpper = upperIndex - index;

  return toLower * discreteValues[upperIndex % discreteValues.size()] +
         toUpper * discreteValues[lowerIndex];
}
}  // namespace eden::interpolation
