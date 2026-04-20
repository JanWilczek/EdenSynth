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

  // waveshaping parameters
  juce::AudioParameterFloat& autoMakeUpGain;

  // output parameters
  juce::AudioParameterFloat& outputVolume;
};
}  // namespace eden::plugin
