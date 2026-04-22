#pragma once
#include <JuceHeader.h>

namespace eden::plugin {
struct ParameterRefs {
  // general parameters
  juce::AudioParameterFloat& pitchBendSemitonesDown;
  juce::AudioParameterFloat& pitchBendSemitonesUp;
  juce::AudioParameterFloat& frequencyOfA4;

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
