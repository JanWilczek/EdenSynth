///
/// \author Jan Wilczek
/// \date 20.11.2018
///
#include "settings/Tuner.h"
#include <cmath>
#include "eden/MidiMessage.h"

namespace eden::settings {
void Tuner::setPitchBendRange(std::pair<int, int> pitchBendRange) {
  _pitchBendRange = pitchBendRange;
}

void Tuner::setFrequencyOfA4(float frequencyOfA4) {
  _frequencyOfA4 = frequencyOfA4;
}

float Tuner::calculatePitch(int midiNoteNumber, int pitchBendValue) {
  return MidiMessage::getMidiNoteInHertz(midiNoteNumber, _frequencyOfA4) *
         std::pow(2.f, getPitchBendInSemitones(pitchBendValue) / 12.f);
}

float Tuner::getPitchBendInSemitones(int pitchBendValue) {
  if (pitchBendValue > PITCH_BEND_NEUTRAL_VALUE) {
    return static_cast<float>(pitchBendValue - PITCH_BEND_NEUTRAL_VALUE) /
           (MAX_PITCH_BEND_VALUE - PITCH_BEND_NEUTRAL_VALUE) *
           _pitchBendRange.second;
  }
  return static_cast<float>(PITCH_BEND_NEUTRAL_VALUE - pitchBendValue) /
         (PITCH_BEND_NEUTRAL_VALUE - MIN_PITCH_BEND_VALUE) *
         _pitchBendRange.first;
}
}  // namespace eden::settings
