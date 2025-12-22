#pragma once
#include <JuceHeader.h>

namespace eden::plugin {
class Parameters {
public:
  [[nodiscard]] static Parameters from(juce::ValueTree);

  [[nodiscard]] juce::var toVar() const { return _impl; }

private:
  explicit Parameters(juce::var v) : _impl{std::move(v)} {}

  juce::var _impl;
};
}  // namespace eden::plugin
