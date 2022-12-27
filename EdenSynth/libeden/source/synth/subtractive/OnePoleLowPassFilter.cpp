///
/// \author Jan Wilczek
/// \date 25.11.2018
///
#include "synth/subtractive/OnePoleLowPassFilter.h"
#include <cmath>
#include "utility/MathConstants.h"

namespace eden::synth::subtractive {
OnePoleLowPassFilter::OnePoleLowPassFilter(float sampleRate)
    : _sampleRate(sampleRate),
      _g(0.f),
      _cutoffFrequency(0.f),
      _feedforwardSample(0.f),
      _feedbackSample(0.f) {}

float OnePoleLowPassFilter::processSample(const float sample) {
  // first stage (feedforward)
  constexpr float multiplyCoeff = 1.f / 1.3f;
  constexpr float feedforwardCoeff = 0.3f / 1.3f;

  const auto firstStageSample =
      sample * multiplyCoeff + _feedforwardSample * feedforwardCoeff;

  _feedforwardSample = sample;

  // second stage (feedback)
  const float beforeG = firstStageSample - _feedbackSample;
  const float afterG = beforeG * _g;

  const float output = afterG + _feedbackSample;

  _feedbackSample = output;

  return output;
}

void OnePoleLowPassFilter::setCutoffFrequency(float cutoffFrequency) {
  _cutoffFrequency = cutoffFrequency;
  calculateG();
}

void OnePoleLowPassFilter::setSampleRate(float sampleRate) {
  _sampleRate = sampleRate;
  calculateG();
}

void OnePoleLowPassFilter::calculateG() {
  const auto omega_c = 2 * static_cast<float>(math_constants::PI) *
                       _cutoffFrequency / _sampleRate;
  _g = 0.9892f * omega_c - 0.4342f * std::pow(omega_c, 2.f) +
       0.1381f * std::pow(omega_c, 3.f) - 0.0202f * std::pow(omega_c, 4.f);
}
}  // namespace eden::synth::subtractive
