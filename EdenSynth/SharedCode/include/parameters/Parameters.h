#pragma once
#include <JuceHeader.h>

namespace eden::plugin {
class Parameters {
public:
  explicit Parameters(juce::ValueTree) {}

  juce::var toVar() const { return {}; }

private:
};
}  // namespace eden::plugin
