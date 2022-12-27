///
/// \author Jan Wilczek
/// \date 25.11.2018
///
#include "synth/subtractive/MoogFilter.h"
#include <algorithm>
#include <cmath>
#include "utility/EdenAssert.h"
#include "utility/MathConstants.h"

namespace eden::synth::subtractive {
MoogFilter::MoogFilter(float sampleRate)
    : _sampleRate(sampleRate),
      _afterA(_sampleRate),
      _afterB(_sampleRate),
      _afterC(_sampleRate),
      _afterD(_sampleRate),
      _c(1.0f),
      _e(0.f),
      _cutoffFrequency(0.f),
      _resonance(0.f),
      _gRes(0.f),
      _feedbackSample(0.f) {}

float MoogFilter::processSample(float sample) {
  float output = 0.f;

  // 1. Summation - control of resonance.
  const auto afterFirstSummation =
      sample - 4.f * _gRes * (_feedbackSample - sample * _gComp);

  // 2. Hyperbolic tangent transfer function.
  const auto transformed = std::tanh(afterFirstSummation);

  // 3. One-pole filters processing
  output += transformed * _a;
  const auto afterFirstLP = _afterA.processSample(transformed);

  output += afterFirstLP * _b;
  const auto afterSecondLP = _afterB.processSample(afterFirstLP);

  output += afterSecondLP * _c;
  const auto afterThirdLP = _afterC.processSample(afterSecondLP);

  output += afterThirdLP * _d;
  const auto afterFourthLP = _afterD.processSample(afterThirdLP);

  _feedbackSample = afterFourthLP;
  output += afterFourthLP * _e;

  return output;
}

void MoogFilter::setCutoffFrequency(float cutoffFrequency) {
  _cutoffFrequency = std::min(cutoffFrequency, _sampleRate / 2.f);
  _afterA.setCutoffFrequency(_cutoffFrequency);
  _afterB.setCutoffFrequency(_cutoffFrequency);
  _afterC.setCutoffFrequency(_cutoffFrequency);
  _afterD.setCutoffFrequency(_cutoffFrequency);

  calculateGRes();
}

void MoogFilter::setResonance(float resonance) {
  _resonance = resonance;

  calculateGRes();
}

void MoogFilter::setPassbandAttenuation(
    PassbandAttenuation passbandAttenuation) {
  switch (passbandAttenuation) {
    case PassbandAttenuation::dB12:
      _c = 1.f;
      _e = 0.f;
      break;
    case PassbandAttenuation::dB24:
      _c = 0.f;
      _e = 1.f;
      break;
    default:
      EDEN_ASSERT(false);
  }
}

void MoogFilter::setSampleRate(float sampleRate) {
  _sampleRate = sampleRate;
  _afterA.setSampleRate(_sampleRate);
  _afterB.setSampleRate(_sampleRate);
  _afterC.setSampleRate(_sampleRate);
  _afterD.setSampleRate(_sampleRate);

  setCutoffFrequency(_cutoffFrequency);
  calculateGRes();
}

void MoogFilter::calculateGRes() {
  const auto omega_c = 1 * static_cast<float>(math_constants::PI) *
                       _cutoffFrequency / _sampleRate;
  _gRes = _resonance *
          (1.0029f + 0.0526f * omega_c - 0.0926f * std::pow(omega_c, 2.f) +
           0.0218f * std::pow(omega_c, 3.f));
}
}  // namespace eden::synth::subtractive
