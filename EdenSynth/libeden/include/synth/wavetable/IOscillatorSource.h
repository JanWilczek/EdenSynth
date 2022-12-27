#pragma once
///
/// \author Jan Wilczek
/// \date 12.11.2018
///
#include <memory>

namespace eden::synth::wavetable {
/// <summary>
/// Interface of an oscillator source.
/// Oscillator source is a sound source which oscillator uses to generate a
/// signal at a given pitch.
/// </summary>
class IOscillatorSource {
 public:
  virtual ~IOscillatorSource() = 0;

  /// <summary>
  /// Each oscillator source should be copyable without exposing its polymorphic
  /// type.
  /// </summary>
  /// <returns>ideal clone of the source</returns>
  virtual std::unique_ptr<IOscillatorSource> clone() = 0;

  /// <summary>
  /// Resets all phase or buffer advance information. Should be called after the
  /// particular note has ended.
  /// </summary>
  virtual void reset() = 0;

  /// <summary>
  /// Activates the oscillator source by setting its pitch.
  /// </summary>
  /// <param name="pitch">frequency in Hz to generate</param>
  virtual void setPitch(float pitch) = 0;

  /// <returns>one sample of generated sound</returns>
  virtual float getSample() = 0;

  virtual void setSampleRate(float sampleRate) = 0;
};

inline IOscillatorSource::~IOscillatorSource() {}
}  // namespace eden::synth::wavetable
