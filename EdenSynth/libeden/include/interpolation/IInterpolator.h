#pragma once
///
/// \author Jan Wilczek
/// \date 20.10.2018
///
#include <vector>

namespace eden::interpolation {
/// <summary>
/// General interpolator interface. Concrete classes may use linear, polynomial
/// or spline interpolation.
/// </summary>
class IInterpolator {
public:
  virtual ~IInterpolator() = 0;

  /// <summary>
  /// Interpolate the value at the <paramref name="index"/> using the <paramref
  /// name="discreteValues"/> as the function to interpolate. The method may
  /// access any of the supplied points.
  /// </summary>
  /// <param name="discreteValues"></param>
  /// <param name="index"></param>
  /// <returns>approximated value</returns>
  virtual float interpolate(const std::vector<float>& discreteValues,
                            float index) = 0;
};

inline IInterpolator::~IInterpolator() {}
}  // namespace eden::interpolation
