///
/// \author Jan Wilczek
/// \date 02.09.2018
///
#include "EdenAdapter.h"
#include "eden/EdenSynthesiser.h"
#include "eden/EnvelopeParameters.h"
#include "eden/MidiBuffer.h"
#include "ParameterIds.h"

namespace eden::plugin {
EdenAdapter::EdenAdapter(
    eden::EdenSynthesiser& synthesiser,
    juce::AudioProcessorValueTreeState& apvts,
    wolfsound::JuceParameterHolder::Builder& parameterBuilder,
    std::filesystem::path assetsPath)
    : _synthesiser(synthesiser),
      _parameters{
          .pitchBendSemitonesDown{
              parameterBuilder.add<juce::AudioParameterFloat>(
                  "pitchBend.semitonesDown",
                  "Pitch bend semitones down",
                  NormalisableRange<float>(-24.f, 0.f, 1.f),
                  -12.f)},
          .pitchBendSemitonesUp{parameterBuilder.add<juce::AudioParameterFloat>(
              "pitchBend.semitonesUp",
              "Pitch bend semitones up",
              NormalisableRange<float>(0.f, 24.f, 1.f),
              2.f)},
          .frequencyOfA4{parameterBuilder.add<juce::AudioParameterFloat>(
              "frequencyOfA4",
              "Frequency of A4",
              NormalisableRange<float>(400.f, 500.f, 0.1f),
              440.f,
              juce::AudioParameterFloatAttributes{}.withLabel("Hz"))},
          .envelopeAdbdrAttackTime{
              parameterBuilder.add<juce::AudioParameterFloat>(
                  "envelope.adbdr.attack.time",
                  "Attack time",
                  NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f),
                  30.f,
                  juce::AudioParameterFloatAttributes{}.withLabel("ms"))},
          .envelopeAdbdrAttackCurve{
              parameterBuilder.add<juce::AudioParameterFloat>(
                  "envelope.adbdr.attack.curve",
                  "Attack curve",
                  NormalisableRange<float>(0.f, 1.f, 1.f),
                  1.f)},
          .envelopeAdbdrDecay1Time{
              parameterBuilder.add<juce::AudioParameterFloat>(
                  "envelope.adbdr.decay1.time",
                  "Decay1 time",
                  NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f),
                  20.f,
                  juce::AudioParameterFloatAttributes{}.withLabel("ms"))},
          .envelopeAdbdrDecay1Curve{
              parameterBuilder.add<juce::AudioParameterFloat>(
                  "envelope.adbdr.decay1.curve",
                  "Decay1 curve",
                  NormalisableRange<float>(0.f, 1.f, 1.f),
                  1.f)},
          .envelopeAdbdrDecay2Time{
              parameterBuilder.add<juce::AudioParameterFloat>(
                  "envelope.adbdr.decay2.time",
                  "Decay2 time",
                  NormalisableRange<float>(1.f, 100000.f, 1.f, 0.3f),
                  20000.f,
                  juce::AudioParameterFloatAttributes{}.withLabel("ms"))},
          .envelopeAdbdrDecay2Curve{
              parameterBuilder.add<juce::AudioParameterFloat>(
                  "envelope.adbdr.decay2.curve",
                  "Decay2 curve",
                  NormalisableRange<float>(0.f, 1.f, 1.f),
                  1.f)},
          .envelopeAdbdrReleaseTime{
              parameterBuilder.add<juce::AudioParameterFloat>(
                  "envelope.adbdr.release.time",
                  "Release time",
                  NormalisableRange<float>(1.f, 40000.f, 1.f, 0.3f),
                  300.f,
                  juce::AudioParameterFloatAttributes{}.withLabel("ms"))},
          .envelopeAdbdrReleaseCurve{
              parameterBuilder.add<juce::AudioParameterFloat>(
                  "envelope.adbdr.release.curve",
                  "Release curve",
                  NormalisableRange<float>(0.f, 1.f, 1.f),
                  1.f)},
          .envelopeAdbdrBreakLevel{
              parameterBuilder.add<juce::AudioParameterFloat>(
                  "envelope.adbdr.breakLevel",
                  "Break level",
                  NormalisableRange<float>(0.f, 1.f, 0.001f, 0.4f),
                  0.6f)},
          .autoMakeUpGain{parameterBuilder.add<juce::AudioParameterFloat>(
              "waveshaper.autoMakeUpGain",
              "Auto make up gain",
              NormalisableRange<float>(0.f, 1.f, 1.f),
              0.f)},
          .waveshaperChebyshevPolynomialOrder{
              parameterBuilder.add<AudioParameterInt>(
                  parameter_id::waveshaperChebyshevPolynomialOrderParameter,
                  "Chebyshev polynomial order",
                  2,
                  10,
                  2)},
          .waveshaperSpreadSeed{parameterBuilder.add<AudioParameterInt>(
              parameter_id::waveshaperSpreadSeedParameter,
              "Waveshaper spread seed",
              0,
              std::numeric_limits<int32_t>::max(),
              0)},
          .waveshaperSpreadValue{parameterBuilder.add<AudioParameterFloat>(
              parameter_id::waveshaperSpreadValueParameter,
              "Waveshaper spread value",
              juce::NormalisableRange{0.f, 1.f, 0.0001f},
              0.f)},
          .outputVolume{parameterBuilder.add<juce::AudioParameterFloat>(
              "output.volume",
              "Global volume",
              NormalisableRange<float>(0.f, 1.f, 0.001f, 0.4f),
              1.0f)},
      },
      _oscillators(_synthesiser, WaveTablePathProvider(assetsPath), 3u),
      _filterParameters(_synthesiser),
      _waveshapingParameters(_synthesiser, apvts) {}

eden::MidiBuffer EdenAdapter::convertToEdenMidi(
    const juce::MidiBuffer& juceMidiBuffer) {
  eden::MidiBuffer edenMidiBuffer;

  for (const auto& midiMessage : juceMidiBuffer) {
    const auto m = midiMessage.getMessage();
    const auto midiEventPos = midiMessage.samplePosition;

    if (m.isNoteOn()) {
      eden::MidiMessage edenMessage(m.getChannel(),
                                    eden::MidiMessage::MidiMessageType::NoteOn);
      edenMessage.setVelocity(m.getFloatVelocity());
      edenMessage.setNoteNumber(m.getNoteNumber());
      edenMidiBuffer.addEvent(std::move(edenMessage), midiEventPos);
    } else if (m.isNoteOff()) {
      eden::MidiMessage edenMessage(
          m.getChannel(), eden::MidiMessage::MidiMessageType::NoteOff);
      edenMessage.setVelocity(m.getFloatVelocity());
      edenMessage.setNoteNumber(m.getNoteNumber());
      edenMidiBuffer.addEvent(std::move(edenMessage), midiEventPos);
    } else if (m.isPitchWheel()) {
      eden::MidiMessage edenMessage(
          m.getChannel(), eden::MidiMessage::MidiMessageType::PitchBendChange);
      edenMessage.setPitchWheelPosition(m.getPitchWheelValue());
      edenMidiBuffer.addEvent(std::move(edenMessage), midiEventPos);
    }
  }

  return edenMidiBuffer;
}

void EdenAdapter::addEdenParameters(
    AudioProcessorValueTreeState& pluginParameters) {
  // oscillator parameters
  _oscillators.addOscillatorParameters(pluginParameters);

  // filter parameters
  _filterParameters.addFilterParameters(pluginParameters);
}

void EdenAdapter::updateEdenParameters(
    const AudioProcessorValueTreeState& pluginParameters) {
  // general parameters
  _synthesiser.setPitchBendRange(
      {static_cast<int>(_parameters.pitchBendSemitonesDown.get()),
       static_cast<int>(_parameters.pitchBendSemitonesUp.get())});
  _synthesiser.setFrequencyOfA4(_parameters.frequencyOfA4.get());

  // oscillator parameters
  _oscillators.updateOscillatorParameters(pluginParameters);

  // filter parameters
  _filterParameters.updateFilterParameters();

  // waveshaping parameters
  _synthesiser.setWaveshapingAutoMakeUpGain(
      static_cast<bool>(_parameters.autoMakeUpGain.get()));
  _waveshapingParameters.updateWaveshapingParameters();

  // ADBDR envelope parameters
  {
    _envelopeParameters->attackTime = std::chrono::milliseconds(
        static_cast<int>(_parameters.envelopeAdbdrAttackTime.get()));

    _envelopeParameters->attackCurve = static_cast<eden::EnvelopeSegmentCurve>(
        static_cast<int>(_parameters.envelopeAdbdrReleaseCurve.get()));

    _envelopeParameters->decay1Time = std::chrono::milliseconds(
        static_cast<int>(_parameters.envelopeAdbdrDecay1Time.get()));

    _envelopeParameters->decay1Curve = static_cast<eden::EnvelopeSegmentCurve>(
        static_cast<int>(_parameters.envelopeAdbdrDecay1Curve.get()));

    _envelopeParameters->decay2Time = std::chrono::milliseconds(
        static_cast<int>(_parameters.envelopeAdbdrDecay2Time.get()));
    _envelopeParameters->decay2Curve = static_cast<eden::EnvelopeSegmentCurve>(
        static_cast<int>(_parameters.envelopeAdbdrDecay2Curve.get()));

    _envelopeParameters->releaseTime = std::chrono::milliseconds(
        static_cast<int>(_parameters.envelopeAdbdrReleaseTime.get()));

    _envelopeParameters->releaseCurve = static_cast<eden::EnvelopeSegmentCurve>(
        static_cast<int>(_parameters.envelopeAdbdrReleaseCurve.get()));

    _envelopeParameters->breakLevel = _parameters.envelopeAdbdrBreakLevel.get();

    _synthesiser.setEnvelopeParameters(_envelopeParameters);
  }

  // output parameters
  _synthesiser.setVolume(_parameters.outputVolume.get());
}

const ParameterRefs& EdenAdapter::parameterRefs() {
  return _parameters;
}

const WaveTablePathProvider& EdenAdapter::getPathProvider() const {
  return _oscillators.getPathProvider();
}

std::shared_ptr<WaveshapingTransferFunctionContainer>
EdenAdapter::getWaveshapingTransferFunction() const noexcept {
  return _waveshapingParameters.getTransferFunctionContainer();
}
}  // namespace eden::plugin
