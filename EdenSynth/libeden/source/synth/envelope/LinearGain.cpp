///
/// \author Jan Wilczek
/// \date 14.10.18
///
#include "synth/envelope/LinearGain.h"
#include "utility/TimeSampleConverter.h"

namespace eden::synth::envelope {
void LinearGain::applyAndUpdateGain(float& currentLevel) {
  currentLevel += _gainPerSample;

  if (currentLevel > float(1.0)) {
    currentLevel = float(1.0);
  } else if (currentLevel < float(0.0)) {
    currentLevel = float(0.0);
  }
}

void LinearGain::calculateGain(float sampleRate,
                               std::chrono::milliseconds duration,
                               float initialLevel,
                               float finalLevel) {
  const auto durationInSamples =
      utility::TimeSampleConverter::timeToSamples(duration, sampleRate);
  _gainPerSample = (finalLevel - initialLevel) / durationInSamples;
}
}  // namespace eden::synth::envelope
