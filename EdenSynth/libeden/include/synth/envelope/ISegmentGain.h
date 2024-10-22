#pragma once
///
/// \author Jan Wilczek
/// \date 14.10.18
///
#include <chrono>

namespace eden::synth::envelope {
/// <summary>
/// Represents the gain changes in time. Enables gain to follow a particular
/// curve (e.g. linear, exponential).
/// </summary>
class ISegmentGain {
public:
  virtual ~ISegmentGain() = 0;

  /// <summary>
  /// Calculates all necessary parameters to make gain follow a certain curve.
  /// </summary>
  /// <param name="sampleRate"></param>
  /// <param name="duration"></param>
  /// <param name="initialLevel"></param>
  /// <param name="finalLevel"></param>
  virtual void calculateGain(float sampleRate,
                             std::chrono::milliseconds duration,
                             float initialLevel,
                             float finalLevel) = 0;

  /// <summary>
  /// Changes current level according to the precalculated parameters.
  /// </summary>
  /// <param name="currentLevel"></param>
  virtual void applyAndUpdateGain(float& currentLevel) = 0;
};

inline ISegmentGain::~ISegmentGain() {}
}  // namespace eden::synth::envelope
