#pragma once
///
/// \author Jan Wilczek
/// \date 12.10.2019
///
#include <random>
#include "synth/wavetable/IOscillatorSource.h"

namespace eden::synth::wavetable {
class WhiteNoiseSource : public IOscillatorSource {
public:
  WhiteNoiseSource(int seed = std::random_device{}());
  ~WhiteNoiseSource() = default;
  WhiteNoiseSource(const WhiteNoiseSource& other);

  std::unique_ptr<IOscillatorSource> clone() override;
  void reset() override;
  void setPitch(float pitch) override;
  float getSample() override;
  void setSampleRate(float) override;

private:
  bool _isPlaying;
  std::default_random_engine _randomEngine;
  std::uniform_real_distribution<float> _uniformDistribution;
};
}  // namespace eden::synth::wavetable
