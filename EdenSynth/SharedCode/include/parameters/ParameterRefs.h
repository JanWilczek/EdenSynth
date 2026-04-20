#pragma once
#include <JuceHeader.h>

namespace eden::plugin {
struct ParameterRefs {
  juce::AudioParameterFloat& pitchBendSemitonesDown;
  juce::AudioParameterFloat& pitchBendSemitonesUp;
};
}  // namespace eden::plugin
