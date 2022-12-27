///
/// \author Jan Wilczek
/// \date 14.10.18
///
#include "synth/envelope/NoGain.h"

namespace eden::synth::envelope {
void NoGain::applyAndUpdateGain(float& currentLevel) {}

void NoGain::calculateGain(float sampleRate,
                           std::chrono::milliseconds duration,
                           float initialLevel,
                           float finalLevel) {}
}  // namespace eden::synth::envelope
