#pragma once
///
/// \author Jan Wilczek
/// \date 27.02.2019
///
#include "synth/wavetable/IOscillatorSource.h"

namespace eden::synth::wavetable {
class SineVASource : public IOscillatorSource {
public:
  SineVASource(float sampleRate);
  ~SineVASource() = default;
  SineVASource(const SineVASource& other);

  float getPitch() const noexcept;

  std::unique_ptr<IOscillatorSource> clone() override;
  void reset() override;
  void setPitch(float pitch) override;
  float getSample() override;
  void setSampleRate(float sampleRate) override;

private:
  float _sampleRate;
  float _pitch;
  float _phase;
  float _delta;
  float _sign;
};
}  // namespace eden::synth::wavetable
