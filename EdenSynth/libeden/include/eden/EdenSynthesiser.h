#pragma once
///
/// \author Jan Wilczek
/// \date 26.08.2018
///
#include <filesystem>
#include <memory>
#include "eden/Oscillator.h"
#include "eden/OscillatorSource.h"
#include "eden_impl/EdenSynthesiserImpl.h"

namespace eden {
class AudioBuffer;
class MidiBuffer;
enum class WaveformGenerator;
struct EnvelopeParameters;

/// <summary>
/// Main Eden synthesiser class.
/// </summary>
class EdenSynthesiser {
public:
  EdenSynthesiser();

  /// <summary>
  /// Fills the given audio buffer with samples according to messages stored in
  /// the MIDI buffer and previous synthesiser state. The audio buffer does not
  /// need to be empty, but its contents will be completely overwritten.
  /// </summary>
  /// <param name="bufferToFill"></param>
  /// <param name="midiBuffer"></param>
  void processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer);

  /// <summary>
  /// Sets a new sample rate of synthesiser.
  /// </summary>
  /// <param name="sampleRate"></param>
  void setSampleRate(float sampleRate);

  /// <returns>currently used sample rate</returns>
  float sampleRate() const noexcept;

  /// <summary>
  /// Sets the expected length of processing block - use it to allocate memory
  /// before calling to <c>processInputBlock()</c>.
  /// </summary>
  /// <param name="samplesPerBlock"></param>
  void setBlockLength(int samplesPerBlock);

  /// <summary>
  /// Sets the overall volume of the synthesiser.
  /// </summary>
  /// <param name="volume">volume in range [0; 1]</param>
  void setVolume(float volume);

  /// <summary>
  /// Sets the frequency which should correspond to MIDI note 69.
  /// </summary>
  /// <param name="frequencyOfA4">A4 frequency in Hz</param>
  void setFrequencyOfA4(float frequencyOfA4);

  /// <summary>
  /// Sets the pitch bend range: how much in semitones can the pitch wheel
  /// transpose a sound. E.g. pair {-12, 2} transposes the pitch by a tone up,
  /// when pitch wheel's value is maximum and by an octave down, when pitch
  /// wheel's value is minimum.
  /// </summary>
  /// <param name="transposeDownTransposeUp">first element indicates the number
  /// of semitones corresponding to maximum down-bend, second the number of
  /// semitones corresponding to maximum up-bend</param>
  void setPitchBendRange(std::pair<int, int> transposeDownTransposeUp);

  /// <summary>
  /// Creates an oscillator source which generates samples in realtime, i.e.
  /// calculates actual function values.
  /// </summary>
  /// <param name="generatorName">name of the generator to base this source
  /// on</param> <returns>handle to the source</returns>
  std::unique_ptr<OscillatorSource> createRealtimeOscillatorSource(
      WaveformGenerator generatorName);

  /// <summary>
  /// Creates an oscillator source which generates samples by interpolating
  /// values given in the <paramref name="waveTable"/>.
  /// </summary>
  /// <param name="waveTable">one cycle of a waveform to be replayed</param>
  /// <returns>handle to the source</returns>
  std::unique_ptr<OscillatorSource> createWaveTableOscillatorSource(
      std::vector<float> waveTable);

  /// <summary>
  /// Creates an oscillator source which generates samples by interpolating
  /// values given in the wave file in path <paramref name="pathToWaveFile"/>.
  /// </summary>
  /// <param name="pathToWaveFile">path to the wave file containing one cycle of
  /// a waveform to replay</param> <returns>handl to the source</returns>
  std::unique_ptr<OscillatorSource> createWaveTableOscillatorSource(
      std::filesystem::path pathToWaveFile);

  /// <summary>
  /// Creates and oscillator based on the given <paramref
  /// name="oscillatorSource"/>. The source can be later altered.
  /// </summary>
  /// <param name="oscillatorSource">source of the waveform to be controlled by
  /// this oscillator</param> <returns>handle to the oscillator</returns>
  std::unique_ptr<Oscillator> createAndAddOscillator(
      std::unique_ptr<OscillatorSource> oscillatorSource);

  /// <summary>
  /// Set cutoff of the pitch-follow low-pass filter.
  /// </summary>
  /// <param name="cutoff">number of harmonic to set the cutoff frequency at (1
  /// is the fundamental), may be non-integer</param>
  void setCutoff(float cutoff);

  /// <summary>
  /// Set resonance or Q of the low-pass filter.Resonance boosts gain at cutoff
  /// frequency of the filter changing into ringing when the value is near 1.
  /// </summary>
  /// <param name="resonance">boost at cutoff frequency in range [0, 1]</param>
  void setResonance(float resonance);

  /// <summary>
  /// Sets the range in which filter's envelope affects its cutoff frequency.
  /// E.g. 1 means that the cutoff frequency will sweep through all frequencies
  /// from 0 to cutoff frequency during attack, while 0 means that filter's
  /// cutoff frequency will be steady (determined solely by the cutoff
  /// parameter, pitch and possibly modulation).
  /// </summary>
  /// <param name="contourAmount">scale in range [0, 1]</param>
  void setContourAmount(float contourAmount);

  /// <summary>
  /// Sets the attenuation of the filter in the pass-band. May be 12 dB per
  /// octave (gentle) or 24 dB per octave (steep).
  /// </summary>
  /// <param name="passbandAttenuation"></param>
  void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);

  /// <summary>
  /// Sets new envelope of the filter's cutoff frequency.
  /// E.g. long attack times create a feeling of the note 'opening up' as the
  /// cutoff frequency gradually increases.
  /// </summary>
  /// <param name="filterEnvelopeParameters">parameters of the filter's envelope
  /// to set - <c>ADSRParameters</c> struct instance for example</param>
  void setFilterEnvelopeParameters(
      std::shared_ptr<EnvelopeParameters> filterEnvelopeParameters);

  void setWaveshapingTransferFunction(std::vector<float> transferFunction);

  void setWaveshapingAutoMakeUpGain(bool makeUpGainEnabled);

  /// <summary>
  /// Sets new envelope of sound - the information about volume change in time
  /// in relation to keyboard events.
  /// </summary>
  /// <param name="envelopeParameters">parameters of the envelope to set -
  /// <c>ADBDRParameters</c> struct instance for example</param>
  void setEnvelopeParameters(
      std::shared_ptr<EnvelopeParameters> envelopeParameters);

private:
  /// <summary>
  /// Internal implementation.
  /// </summary>
  std::unique_ptr<EdenSynthesiserImpl> _impl;
};
}  // namespace eden
