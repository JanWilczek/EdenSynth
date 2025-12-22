#pragma once
#include <JuceHeader.h>

namespace eden::plugin {
class Parameters {
public:
  [[nodiscard]] static Parameters from(juce::ValueTree);
  [[nodiscard]] static Parameters from(juce::var);

  [[nodiscard]] juce::var toVar() const { return _impl; }

private:
  explicit Parameters(juce::var v) : _impl{std::move(v)} {}

  juce::var _impl;
};

void updateApvts(juce::AudioProcessorValueTreeState&, const Parameters&);
}  // namespace eden::plugin
