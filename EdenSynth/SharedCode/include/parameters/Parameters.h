#pragma once
#include <optional>
#include <JuceHeader.h>

namespace eden::plugin {
class Parameters {
public:
  [[nodiscard]] static Parameters from(const juce::var&);

  [[nodiscard]] static Parameters from(const juce::Array<juce::var>&);

  [[nodiscard]] static std::optional<Parameters> fromChecked(const juce::var&);
  [[nodiscard]] static std::optional<Parameters> fromChecked(
      const juce::Array<juce::var>&);

  [[nodiscard]] const juce::var& toVar() const noexcept { return _impl; }
  [[nodiscard]] const juce::Array<juce::var>& toVarArray() const noexcept {
    return *_impl.getProperty("parameters", juce::var{juce::Array<juce::var>{}})
                .getArray();
  }

private:
  explicit Parameters(juce::var v) : _impl{std::move(v)} {}

  juce::var _impl;
};
}  // namespace eden::plugin
