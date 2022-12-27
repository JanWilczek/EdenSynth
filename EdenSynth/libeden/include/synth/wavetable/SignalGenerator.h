#pragma once
///
/// \author Jan Wilczek
/// \date 08.10.2018
///
#include <map>
#include "eden/OscillatorParameters.h"
#include "synth/wavetable/SynthOscillator.h"

namespace eden::synth::wavetable {
/// <summary>
/// Signal generator module. Responsible for creating the initial signal, which
/// can be further modified. It is the "source" part of the "source and
/// modifier" synthesis idiom.
/// </summary>
class SignalGenerator {
 public:
  /// <summary>
  /// Creates the generator with the default sine timbre.
  /// </summary>
  SignalGenerator() = default;

  /// <summary>
  /// Fills the <paramref name="audioChannel"/> from <paramref
  /// name="startSample"/> to <paramref name="startSample"/> + <paramref
  /// name="samplesToGenerate"/> (excluding) with currently set timbre at
  /// currently set pitch. Whatever has been on the channel before will be
  /// overwritten.
  /// </summary>
  /// <param name="audioChannel">channel to fill in specified range</param>
  /// <param name="startSample">the first index to fill</param>
  /// <param name="samplesToGenerate">number of samples to generate</param>
  void generateSignal(float* audioChannel,
                      int startSample,
                      int samplesToGenerate);

  /// <summary>
  /// Stops generating the signal - all subsequent calls to
  /// <c>generateSignal</c> will fill the channel with zeros until new pitch is
  /// set.
  /// </summary>
  void stop();

  /// <summary>
  /// Sets the pitch of the generator. From now on all calls to generateSignal
  /// will fill the given channel with samples at specified pitch.
  /// </summary>
  /// <param name="pitch"></param>
  void setPitch(float pitch);

  /// <summary>
  /// Adds given <paramref name="oscillator"/> to the sound generators. It will
  /// be used to generate sound upon the calls to the <see
  /// cref="generateSignal"/>.
  /// </summary>
  /// <param name="oscillator"></param>
  void addOscillator(SynthOscillator oscillator);

  /// <summary>
  /// Removes oscillator with the given id from sound generation.
  /// </summary>
  /// <param name="oscillatorToRemove">id of the oscillator to remove</param>
  void removeOscillator(OscillatorId oscillatorToRemove);

  /// <summary>
  /// Sets oscillator source of the oscillator with the given id.
  /// </summary>
  /// <param name="oscillatorId"></param>
  /// <param name="source"></param>
  void setOscillatorSource(OscillatorId oscillatorId,
                           std::unique_ptr<IOscillatorSource> source);

  /// <summary>
  /// Sets octave transposition of the oscillator with the given id.
  /// </summary>
  /// <param name="oscillatorId"></param>
  /// <param name="octaveShift"></param>
  void setOctaveTransposition(OscillatorId oscillatorId, int octaveShift);

  /// <summary>
  /// Sets semitone transposition of the oscillator with the given id.
  /// </summary>
  /// <param name="oscillatorId"></param>
  /// <param name="semitoneShift"></param>
  void setSemitoneTransposition(OscillatorId oscillatorId, int semitoneShift);

  /// <summary>
  /// Sets cent transposition of the oscillator with the given id.
  /// </summary>
  /// <param name="oscillatorId"></param>
  /// <param name="centShift"></param>
  void setCentTransposition(OscillatorId oscillatorId, int centShift);

  /// <summary>
  /// Sets volume of the oscillator with the given id.
  /// </summary>
  /// <param name="oscillatorId"></param>
  /// <param name="volume"></param>
  void setOscillatorVolume(OscillatorId oscillatorId, float volume);

  /// <summary>
  /// Sets on or off the oscillator with the given id.
  /// </summary>
  /// <param name="oscillatorId"></param>
  /// <param name="isOn">true to turn the oscillator on, false to turn it
  /// off</param>
  void setOscillatorOn(OscillatorId oscillatorId, bool isOn);

  void setSampleRate(float sampleRate);

 private:
  /// <summary>
  /// Generates one sample at specified sample index and overwrites the
  /// pre-existing value with new one based on current pitch. Advances in phase.
  /// </summary>
  /// <param name="audioChannel"></param>
  /// <param name="sampleIndex"></param>
  void generateSample(float* audioChannel, int sampleIndex);

  std::map<OscillatorId, SynthOscillator> _oscillators;
};
}  // namespace eden::synth::wavetable
