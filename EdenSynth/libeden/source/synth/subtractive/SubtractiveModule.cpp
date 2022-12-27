///
///	\author Jan Wilczek
///	 \date 08.10.2018
///
#include "synth/subtractive/SubtractiveModule.h"
#include "eden/EnvelopeParameters.h"
#include "synth/envelope/ADSR.h"

namespace eden::synth::subtractive {
SubtractiveModule::SubtractiveModule(float sampleRate)
    : _filter(sampleRate),
      _cutoffFrequencyEnvelope(
          std::make_shared<envelope::ADSR>(sampleRate, ADSRParameters{})),
      _cutoff(0.f),
      _contourAmount(1.f),
      _pitch(0.f) {}

void SubtractiveModule::process(float* audioChannel,
                                int startSample,
                                int samplesToProcess) {
  for (auto sample = startSample; sample < startSample + samplesToProcess;
       ++sample) {
    setCutoffFrequency();
    audioChannel[sample] = _filter.processSample(audioChannel[sample]);
  }
}

void SubtractiveModule::keyOn() {
  _cutoffFrequencyEnvelope->keyOn();
}

void SubtractiveModule::keyOff() {
  _cutoffFrequencyEnvelope->keyOff();
}

void SubtractiveModule::setCutoff(float cutoff) {
  _cutoff = cutoff;
  setCutoffFrequency();
}

void SubtractiveModule::setResonance(float resonance) {
  _filter.setResonance(resonance);
}

void SubtractiveModule::setContourAmount(float contourAmount) {
  _contourAmount = contourAmount;
}

void SubtractiveModule::setPassbandAttenuation(
    PassbandAttenuation passbandAttenuation) {
  _filter.setPassbandAttenuation(passbandAttenuation);
}

void SubtractiveModule::setEnvelope(
    std::shared_ptr<envelope::Envelope> envelope) {
  _cutoffFrequencyEnvelope = envelope;
}

std::shared_ptr<envelope::Envelope> SubtractiveModule::getEnvelope()
    const noexcept {
  return _cutoffFrequencyEnvelope;
}

void SubtractiveModule::setSampleRate(float sampleRate) {
  _filter.setSampleRate(sampleRate);
}

void SubtractiveModule::setPitch(float pitch) {
  _pitch = pitch;
  setCutoffFrequency();
}

void SubtractiveModule::setCutoffFrequency() {
  auto newCutoffFrequency = _pitch * _cutoff;

  // apply squeezed envelope
  float envelopeValue = 1.f;
  _cutoffFrequencyEnvelope->apply(envelopeValue);
  newCutoffFrequency *= (_contourAmount * envelopeValue + 1.f - _contourAmount);

  _filter.setCutoffFrequency(newCutoffFrequency);
}
}  // namespace eden::synth::subtractive
