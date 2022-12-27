///
/// \author Jan Wilczek
/// \date 14.10.18
///
#include "synth/envelope/ExponentialGain.h"
#include <algorithm>
#include <cmath>
#include "utility/MathConstants.h"
#include "utility/TimeSampleConverter.h"

namespace eden::synth::envelope {
void ExponentialGain::calculateGain(float sampleRate,
                                    std::chrono::milliseconds duration,
                                    float initialLevel,
                                    float finalLevel) {
  /*
   * This gain should be 'exponentially linear', which means that plot in dB
   * scale should be linear. E(n) - actual envelope in linear values. EdB(n) -
   * values of envelope in dB which create a linear plot. n is the number of
   * sample.
   *
   * EdB(n) should be linear, so:
   * EdB(n) = a * n + b,		(1)
   * where a and b are some constants.
   *
   * On the other hand:
   * EdB(n) = 20 * log10(E(n))	(2)
   * by the definition.
   *
   * Comparing (1) against (2) gives an equation from which one can calculate
   * that E(n) = 10 ^ (a * n / 20) * 10 ^ (b / 20),		(3) so current
   * envelope value should be multiplied by 10 ^ (a / 20) in every update.
   *
   * a and b can be calculated by solving following equations:
   * E(0) = 10 ^ (b / 20) = initialLevel,
   * E(durationInSamples) = 10 ^ (a * durationInSamples / 20) * 10 ^ (b / 20) =
   * 10 ^ (a * durationInSamples / 20) * initialLevel = finalLevel. So: 10 ^ (a
   * * durationInSamples / 20) = finalLevel / initialLevel a * durationInSamples
   * / 20 = log10(finalLevel / initialLevel) a = (20 / durationInSamples) *
   * log10(finalLevel  / initialLevel)
   */

  const auto durationInSamples =
      utility::TimeSampleConverter::timeToSamples(duration, sampleRate);

  initialLevel = std::max(initialLevel, math_constants::THRESHOLD_OF_HEARING);
  finalLevel = std::max(finalLevel, math_constants::THRESHOLD_OF_HEARING);

  const auto exponent = (1.0 / durationInSamples) *
                        std::log10(finalLevel / initialLevel);  // = a / 20
  _multiplier = static_cast<float>(std::pow(10, exponent));
}

void ExponentialGain::applyAndUpdateGain(float& currentGain) {
  if (currentGain < math_constants::THRESHOLD_OF_HEARING) {
    currentGain = math_constants::THRESHOLD_OF_HEARING;
  } else {
    currentGain *= _multiplier;

    if (currentGain > 1.0f) {
      currentGain = 1.0f;
    }
  }
}
}  // namespace eden::synth::envelope
