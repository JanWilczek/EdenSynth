#pragma once
#include <JuceHeader.h>
#include <array>

namespace eden::plugin {
struct OscillatorParameters {
  std::string name;
  juce::AudioParameterFloat& isRealTime;
  juce::AudioParameterFloat& waveTable;
  juce::AudioParameterFloat& generatorName;
  juce::AudioParameterFloat& octaveTransposition;
  juce::AudioParameterFloat& semitoneTransposition;
  juce::AudioParameterFloat& centTransposition;
  juce::AudioParameterFloat& volume;
  juce::AudioParameterFloat& on;
};

using OscillatorParametersContainer = std::array<OscillatorParameters, 3>;

struct ParameterRefs {
  // general parameters
  juce::AudioParameterFloat& pitchBendSemitonesDown;
  juce::AudioParameterFloat& pitchBendSemitonesUp;
  juce::AudioParameterFloat& frequencyOfA4;

  // oscillator parameters
  OscillatorParametersContainer oscillators;

  // ADBDR envelope parameters
  juce::AudioParameterFloat& envelopeAdbdrAttackTime;
  juce::AudioParameterFloat& envelopeAdbdrAttackCurve;
  juce::AudioParameterFloat& envelopeAdbdrDecay1Time;
  juce::AudioParameterFloat& envelopeAdbdrDecay1Curve;
  juce::AudioParameterFloat& envelopeAdbdrDecay2Time;
  juce::AudioParameterFloat& envelopeAdbdrDecay2Curve;
  juce::AudioParameterFloat& envelopeAdbdrReleaseTime;
  juce::AudioParameterFloat& envelopeAdbdrReleaseCurve;
  juce::AudioParameterFloat& envelopeAdbdrBreakLevel;

  // filter parameters
  juce::AudioParameterFloat& filterCutoff;
  juce::AudioParameterFloat& filterResonance;
  juce::AudioParameterFloat& filterContourAmount;
  juce::AudioParameterFloat& filterPassbandAttenuation;
  juce::AudioParameterFloat& filterAttackTime;
  juce::AudioParameterFloat& filterDecayTime;
  juce::AudioParameterFloat& filterSustainLevel;
  juce::AudioParameterFloat& filterReleaseTime;

  // waveshaping parameters
  juce::AudioParameterFloat& autoMakeUpGain;
  juce::AudioParameterChoice& waveshaperCurve;
  juce::AudioParameterInt& waveshaperChebyshevPolynomialOrder;
  juce::AudioParameterInt& waveshaperSpreadSeed;
  juce::AudioParameterFloat& waveshaperSpreadValue;

  // output parameters
  juce::AudioParameterFloat& outputVolume;
};
}  // namespace eden::plugin
