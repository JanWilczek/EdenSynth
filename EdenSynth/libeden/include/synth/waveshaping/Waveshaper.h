#pragma once
///
/// \author Jan Wilczek
/// \date 03.12.2018
///
#include <memory>
#include <vector>
#include "interpolation/IInterpolator.h"

namespace eden::synth::waveshaping {
class Waveshaper {
 public:
  Waveshaper();

  float transform(float sample);

  void setTransferFunction(std::vector<float> transferFunction);

  void setAutoMakeUpGain(bool makeUpGainEnabled);

 private:
  bool _makeUpGainEnabled;
  std::vector<float> _transferFunction;
  float _makeUpGainFactor;
  std::unique_ptr<interpolation::IInterpolator> _interpolator;
};
}  // namespace eden::synth::waveshaping
