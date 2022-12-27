#pragma once
///
/// \author Jan Wilczek
/// \date 14.10.18
///
#include "synth/envelope/ISegmentGain.h"

namespace eden::synth::envelope {
/// <summary>
/// Represents exponential gain curve - i.e. linear to the human ear.
/// </summary>
class ExponentialGain : public ISegmentGain {
 public:
  ~ExponentialGain() = default;

  void calculateGain(float sampleRate,
                     std::chrono::milliseconds duration,
                     float initialLevel,
                     float finalLevel) override;
  void applyAndUpdateGain(float& currentGain) override;

 private:
  float _multiplier = float(1);
};
}  // namespace eden::synth::envelope
