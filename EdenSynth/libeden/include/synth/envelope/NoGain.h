#pragma once
///
/// \author Jan Wilczek
/// \date 14.10.18
///
#include "synth/envelope/ISegmentGain.h"

namespace eden::synth::envelope {
/// <summary>
/// Represents no change in gain - the given value will remain constant.
/// </summary>
class NoGain : public ISegmentGain {
 public:
  ~NoGain() = default;

  void calculateGain(float sampleRate,
                     std::chrono::milliseconds duration,
                     float initialLevel,
                     float finalLevel) override;
  void applyAndUpdateGain(float& currentGain) override;
};
}  // namespace eden::synth::envelope
