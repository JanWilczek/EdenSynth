///
/// \author Jan Wilczek
/// \date 29.08.2018
///
#include "eden/EdenSynthesiser.h"
#include "eden/AudioBuffer.h"
#include "eden/EnvelopeParameters.h"
#include "eden/MidiBuffer.h"

namespace eden {
EdenSynthesiser::EdenSynthesiser()
    : _impl(std::make_unique<EdenSynthesiserImpl>()) {}

void EdenSynthesiser::processInputBlock(AudioBuffer& bufferToFill,
                                        MidiBuffer& midiBuffer) {
  _impl->processInputBlock(bufferToFill, midiBuffer);
}

void EdenSynthesiser::setSampleRate(float sampleRate) {
  _impl->setSampleRate(sampleRate);
}

float EdenSynthesiser::sampleRate() const noexcept {
  return _impl->sampleRate();
}

void EdenSynthesiser::setBlockLength(int samplesPerBlock) {
  _impl->setBlockLength(samplesPerBlock);
}

void EdenSynthesiser::setVolume(float volume) {
  _impl->setVolume(volume);
}

void EdenSynthesiser::setFrequencyOfA4(float frequencyOfA4) {
  _impl->setFrequencyOfA4(frequencyOfA4);
}

void EdenSynthesiser::setPitchBendRange(
    std::pair<int, int> transposeDownTransposeUp) {
  _impl->setPitchBendRange(transposeDownTransposeUp);
}

std::unique_ptr<OscillatorSource>
EdenSynthesiser::createRealtimeOscillatorSource(
    WaveformGenerator generatorName) {
  return _impl->createRealtimeOscillatorSource(generatorName);
}

std::unique_ptr<OscillatorSource>
EdenSynthesiser::createWaveTableOscillatorSource(std::vector<float> waveTable) {
  return _impl->createWaveTableOscillatorSource(waveTable);
}

std::unique_ptr<OscillatorSource>
EdenSynthesiser::createWaveTableOscillatorSource(
    std::filesystem::path pathToWaveFile) {
  return _impl->createWaveTableOscillatorSource(pathToWaveFile);
}

std::unique_ptr<Oscillator> EdenSynthesiser::createAndAddOscillator(
    std::unique_ptr<OscillatorSource> oscillatorSource) {
  return _impl->createAndAddOscillator(std::move(oscillatorSource));
}

void EdenSynthesiser::setCutoff(float cutoff) {
  _impl->setCutoff(cutoff);
}

void EdenSynthesiser::setResonance(float resonance) {
  _impl->setResonance(resonance);
}

void EdenSynthesiser::setContourAmount(float contourAmount) {
  _impl->setContourAmount(contourAmount);
}

void EdenSynthesiser::setPassbandAttenuation(
    PassbandAttenuation passbandAttenuation) {
  _impl->setPassbandAttenuation(passbandAttenuation);
}

void EdenSynthesiser::setFilterEnvelopeParameters(
    std::shared_ptr<EnvelopeParameters> filterEnvelopeParameters) {
  _impl->setFilterEnvelopeParameters(filterEnvelopeParameters);
}

void EdenSynthesiser::setWaveshapingTransferFunction(
    std::vector<float> transferFunction) {
  _impl->setWaveshapingTransferFunction(std::move(transferFunction));
}

void EdenSynthesiser::setWaveshapingAutoMakeUpGain(bool makeUpGainEnabled) {
  _impl->setWaveshapingAutoMakeUpGain(makeUpGainEnabled);
}

void EdenSynthesiser::setEnvelopeParameters(
    std::shared_ptr<EnvelopeParameters> envelopeParameters) {
  _impl->setEnvelopeParameters(envelopeParameters);
}
}  // namespace eden
