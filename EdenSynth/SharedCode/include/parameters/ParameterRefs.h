#pragma once
#include <JuceHeader.h>

namespace eden::plugin {
struct ParameterRefs {
  juce::AudioParameterFloat& pitchBendSemitonesDown;
  juce::AudioParameterFloat& pitchBendSemitonesUp;
  juce::AudioParameterFloat& frequencyOfA4;
};
}  // namespace eden::plugin
