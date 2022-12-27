#pragma once
///
/// \author Jan Wilczek
/// \date 11.11.2018
///
#include <memory>
#include "eden/OscillatorParameters.h"
#include "eden_impl/OscillatorImpl.h"

namespace eden {
class OscillatorSource;

/// <summary>
/// Class representing an oscillator - a single waveform generator capable of
/// generating continuous signals at particular frequency. Oscillators can be
/// stacked so that they produce complex timbres.
/// </summary>
class Oscillator {
 public:
  Oscillator(std::unique_ptr<OscillatorImpl> impl);

  /// <returns>unique id assigned to this oscillator</returns>
  OscillatorId getId() const noexcept;

  /// <summary>
  /// Sets the source (timbre, waveform) of this oscillator.
  /// </summary>
  /// <param name="source">oscillator source created by one of the
  /// <c>EdenSynthesiser</c>'s methods.</param>
  void setSource(std::unique_ptr<OscillatorSource> source);

  /// <summary>
  /// Sets pitch transposition in octaves in regard to the pitch set by the
  /// <c>setPitch()</c> method.
  /// </summary>
  /// <param name="octaveShift">e.g. +1 means, that the rendered sound will be
  /// one octave higher than set</param>
  void setOctaveTransposition(int octaveShift);

  /// <summary>
  /// Sets pitch transposition in semitones in regard to the pitch set by the
  /// <c>setPitch()</c> method.
  /// </summary>
  /// <param name="semitoneShift">e.g. -2 means, that the rendered sound will be
  /// one tone lower than set</param>
  void setSemitoneTransposition(int semitoneShift);

  /// <summary>
  /// Sets pitch transposition in cents in regard to the pitch set by the
  /// <c>setPitch()</c> method.
  /// </summary>
  /// <param name="centShift">e.g. +50 means, that the rendered sound will be
  /// one quarter of a tone higher than set</param>
  void setCentTransposition(int centShift);

  /// <param name="volume">volume in range [0, 1]</param>
  void setVolume(float volume) noexcept(false);

  /// <param name="isOn">true to turn the oscillator on, false to turn it off -
  /// after the latter <c>getSample()</c> will return zero at each call</param>
  void setOn(bool isOn);

 private:
  std::unique_ptr<OscillatorImpl> _impl;
};
}  // namespace eden
