///
/// \author Jan Wilczek
/// \date 27.11.2018
///
#include "synth/envelope/EnvelopeGenerator.h"

namespace eden::synth::envelope {
EnvelopeGenerator::EnvelopeGenerator(std::shared_ptr<Envelope> envelope)
    : _envelope(std::move(envelope)), _callback([]() {}) {}

void EnvelopeGenerator::apply(float* channel,
                              int startSample,
                              int samplesToApply) {
  _envelope->applyInRange(channel, startSample, samplesToApply);
}

void EnvelopeGenerator::keyOn() {
  _envelope->keyOn();
}

void EnvelopeGenerator::keyOff() {
  _envelope->keyOff();
}

std::shared_ptr<Envelope> EnvelopeGenerator::getEnvelope() const noexcept {
  return _envelope;
}

void EnvelopeGenerator::setEnvelope(std::shared_ptr<Envelope> envelope) {
  _envelope = envelope;
  _envelope->setOnEnvelopeEndedCallback(_callback);
}

void EnvelopeGenerator::setOnEnvelopeEndedCallback(
    Envelope::OnEnvelopeEnded callback) {
  _callback = callback;
  _envelope->setOnEnvelopeEndedCallback(_callback);
}
}  // namespace eden::synth::envelope
