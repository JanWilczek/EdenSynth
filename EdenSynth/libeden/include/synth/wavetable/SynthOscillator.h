#pragma once
///
/// \author Jan Wilczek
/// \date 12.11.2018
///
#include <memory>
#include "eden/OscillatorParameters.h"
#include "synth/wavetable/IOscillatorSource.h"

namespace eden::synth::wavetable {
/// <summary>
/// Class representing an actual oscillator. It contains information about
/// pitch, transposition, volume and on/off status.
/// </summary>
class SynthOscillator {
 public:
  SynthOscillator() = default;
  SynthOscillator(OscillatorId id,
                  std::unique_ptr<IOscillatorSource> oscillatorSource);
  SynthOscillator(const SynthOscillator&);
  SynthOscillator(SynthOscillator&&) noexcept;
  SynthOscillator& operator=(const SynthOscillator&);
  SynthOscillator& operator=(SynthOscillator&&) noexcept;

  /// <returns>id of this oscillator</returns>
  OscillatorId getId() const noexcept;

  /// <summary>
  /// Generates and returns one sample.
  /// </summary>
  /// <returns>generated sample</returns>
  float getSample();

  /// <summary>
  /// Resets the note being played. Should be called when the voice has ended.
  /// </summary>
  void reset();

  /// <param name="oscillatorSource">source to base the generated sound
  /// on</param>
  void setSource(std::unique_ptr<IOscillatorSource> oscillatorSource);

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

  /// <param name="pitch">frequency in Hz</param>
  void setPitch(float pitch);

  /// <param name="volume">volume in range [0, 1]</param>
  void setVolume(float volume);

  /// <param name="isOn">true to turn the oscillator on, false to turn it off -
  /// after the latter <c>getSample()</c> will return zero at each call</param>
  void setOn(bool isOn);

  bool isOn() const noexcept;

  void setSampleRate(float sampleRate);

  /// <returns>actual pitch played (i.e. set pitch after
  /// transposition)</returns>
  float pitch();

 private:
  /// <summary>
  /// Sets the pitch of the oscilator based on <c>pitch()</c> return value.
  /// </summary>
  void setOscillatorPitch();

  /// <summary>
  /// Source of this oscillator.
  /// </summary>
  std::unique_ptr<IOscillatorSource> _oscillatorSource;

  /// <summary>
  /// Unique ID of this oscillator.
  /// </summary>
  OscillatorId _id;

  /// <summary>
  /// Transposition in octaves.
  /// </summary>
  int _octaveShift;

  /// <summary>
  /// Transposition in semitones
  /// </summary>
  int _semitoneShift;

  /// <summary>
  /// Transposition in cents.
  /// </summary>
  int _centShift;

  /// <summary>
  /// Volume in range [0, 1].
  /// </summary>
  float _volume;

  /// <summary>
  /// True if on, false otherwise.
  /// </summary>
  bool _isOn;

  /// <summary>
  /// Pitch set by the call to <c>setPitch()</c>.
  /// </summary>
  float _originalPitch;
};
}  // namespace eden::synth::wavetable
