#pragma once
///
/// \author Jan Wilczek
/// \date 20.11.2018
///
#include <utility>

namespace eden::settings {
class Tuner {
public:
  /// <summary>
  /// MIDI-specified "zero", "centre" or "neutral" pitch bend position.
  /// </summary>
  static constexpr int PITCH_BEND_NEUTRAL_VALUE = 0x2000;

  /// <summary>
  /// MIDI-specified maximum pitch bend value.
  /// </summary>
  static constexpr int MAX_PITCH_BEND_VALUE = 0x3FFF;

  /// <summary>
  /// MIDI-specified minimum pitch bend value.
  /// </summary>
  static constexpr int MIN_PITCH_BEND_VALUE = 0x0000;

  /// <summary>
  /// Sets the upper and lower range of pitch bend.
  /// </summary>
  /// <param name="pitchBendRange">first value determines how many semitones up
  /// (positive value) or down (negative value) the tone will be bent in the
  /// minimum pitch wheel position, second - how many semitones up (positive
  /// value) or down (negative value) the tone will be bent in the maximum pitch
  /// wheel position</param>
  void setPitchBendRange(std::pair<int, int> pitchBendRange);

  /// <summary>
  /// Sets the generated frequency of A4 note on the keyboard (note number 69 in
  /// MIDI specification).
  /// </summary>
  /// <param name="frequencyOfA4"></param>
  void setFrequencyOfA4(float frequencyOfA4);

  /// <param name="midiNoteNumber"></param>
  /// <param name="pitchBendValue"></param>
  /// <returns>frequency in Hz calculated from the MIDI note number, pitch bend
  /// value and set A4 key frequency</returns>
  float calculatePitch(int midiNoteNumber, int pitchBendValue);

  /// <param name="pitchBendValue"></param>
  /// <returns>how many semitones up or down corresponds to the given pitch bend
  /// value based on current pitch bend range</returns>
  float getPitchBendInSemitones(int pitchBendValue);

private:
  /// <summary>
  /// Current A4 frequency (note number 69 in the MIDI standard).
  /// </summary>
  float _frequencyOfA4 = 440.f;

  /// <summary>
  /// Current pitch bend range.
  /// First value determines how many semitones up (positive value) or down
  /// (negative value) corresponds to minimal pitch bend value. Second value
  /// determines how many semitones up (positive value) or down (negative value)
  /// corresponds to maximal pitch bend value.
  /// </summary>
  std::pair<int, int> _pitchBendRange = {-12, 2};
};
}  // namespace eden::settings
