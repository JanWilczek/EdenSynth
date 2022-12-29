#pragma once
///
/// \author Jan Wilczek
/// \date 11.10.2018
///
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope {
/// <summary>
/// Represents attack - time segment when volume should increase.
/// </summary>
class Attack : public EnvelopeSegment {
public:
  Attack(float sampleRate,
         std::unique_ptr<ISegmentGain> envelopeGain,
         std::chrono::milliseconds duration,
         float initialLevel = 0,
         float finalLevel = 1.0);
  ~Attack() override = default;

  bool hasEnded(float currentLevel) override;
};
}  // namespace eden::synth::envelope
