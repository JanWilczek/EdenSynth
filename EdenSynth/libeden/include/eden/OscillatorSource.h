#pragma once
///
/// \author Jan Wilczek
/// \date 12.11.2018
///
#include <memory>
#include "eden/OscillatorParameters.h"
#include "eden_impl/OscillatorSourceImpl.h"

namespace eden {
enum class WaveformGenerator;

/// <summary>
/// Class representing an oscillator source - source of sound for oscillator.
/// It may be realtime (each sample is created using a mathematical formula) or
/// wavetable (where waveforms are interpolated from existing ones).
/// </summary>
class OscillatorSource {
 public:
  OscillatorSource(std::unique_ptr<OscillatorSourceImpl> impl);

  /// <returns>unique id of this oscillator source</returns>
  OscillatorSourceId getId();

 private:
  std::unique_ptr<OscillatorSourceImpl> _impl;
};
}  // namespace eden
