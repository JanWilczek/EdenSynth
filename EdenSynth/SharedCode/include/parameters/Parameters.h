#pragma once
#include <optional>
#include <JuceHeader.h>

namespace eden::plugin {
class Parameters {
public:
  [[nodiscard]] static Parameters from(juce::ValueTree);

  [[nodiscard]] static Parameters from(const juce::var&);

  [[nodiscard]] static Parameters from(const juce::Array<juce::var>&);

  [[nodiscard]] static std::optional<Parameters> fromChecked(const juce::var&);
  [[nodiscard]] static std::optional<Parameters> fromChecked(
      const juce::Array<juce::var>&);

  [[nodiscard]] juce::var toVar() const { return _impl; }
  [[nodiscard]] juce::Array<juce::var> toVarArray() const {
    return *_impl.getProperty("parameters", juce::var{juce::Array<juce::var>{}})
                .getArray();
  }

private:
  explicit Parameters(juce::var v) : _impl{std::move(v)} {}

  juce::var _impl;
};

void updateApvts(juce::AudioProcessorValueTreeState&, const Parameters&);
}  // namespace eden::plugin
