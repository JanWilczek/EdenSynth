#pragma once
///
/// \author Jan Wilczek
/// \date 26.02.2019
///
#include "synth/wavetable/IOscillatorSource.h"
#include "synth/wavetable/va_sources/SawtoothVASource.h"

namespace eden::synth::wavetable {
/// <summary>
/// Oscillator generating a Virtual Analog square waveform based on the
/// Differential Parabolic Waveform (DPW) algorithm proposed by Valimaki and
/// Huovilainen.
/// </summary>
class PulseVASource : public IOscillatorSource {
 public:
  PulseVASource(float sampleRate);
  ~PulseVASource() = default;
  PulseVASource(const PulseVASource& other);

  /// <summary>
  /// Sets the duty cycle of the waveform - relative time period in which
  /// the signal will be above 0. 50 % duty cycle gives the square waveform.
  /// </summary>
  /// <param name="dutyCycle">in range [0, 1]</param>
  void setDutyCycle(float dutyCycle);

  std::unique_ptr<IOscillatorSource> clone() override;
  void reset() override;
  void setPitch(float pitch) override;
  float getSample() override;
  void setSampleRate(float sampleRate) override;

 private:
  SawtoothVASource _saw;
  SawtoothVASource _sawShifted;
};
}  // namespace eden::synth::wavetable
