///
/// \author Jan Wilczek
/// \date 11.10.2018
///
#include "synth/envelope/Envelope.h"
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope {
Envelope::~Envelope() {}

void Envelope::apply(float& sample) {
  sample *= _currentLevel;
  updateGain();

  if (_segments[_currentSegment]->hasEnded(_currentLevel)) {
    switchToSegment(_currentSegment + 1);
    if (hasEnded()) {
      _currentLevel = 0.0;
      _onEnvelopeEndedCallback();
    }
  }
}

void Envelope::applyInRange(float* samples,
                            int startSample,
                            int samplesToApply) {
  for (auto sample = startSample; sample < startSample + samplesToApply;
       ++sample) {
    apply(samples[sample]);
  }
}

void Envelope::setSampleRate(float sampleRate) {
  for (auto& segment : _segments) {
    segment->setSampleRate(sampleRate);
  }
}

void Envelope::setOnEnvelopeEndedCallback(OnEnvelopeEnded callback) {
  _onEnvelopeEndedCallback = callback;
}

void Envelope::switchToSegment(size_t segment) {
  _currentSegment = segment;
}

void Envelope::updateGain() {
  _segments[_currentSegment]->applyAndUpdateGain(_currentLevel);
}
}  // namespace eden::synth::envelope
