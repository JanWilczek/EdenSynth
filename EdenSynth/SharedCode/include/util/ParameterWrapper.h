#pragma once

#include <memory>
#include <juce_audio_processors/juce_audio_processors.h>

namespace eden::plugin::util {
template <class ParameterT>
class ParameterWrapper {
public:
  template <typename... Args>
  ParameterWrapper(juce::AudioProcessorValueTreeState& apvts, Args... args)
      : p{createAndAddParameter(apvts, std::forward<Args>(args)...)} {}

  const ParameterT& get() const noexcept { return *p; }
  ParameterT& get() noexcept { return *p; }

private:
  template <typename... Args>
  static ParameterT* createAndAddParameter(
      juce::AudioProcessorValueTreeState& apvts,
      Args... args) {
    auto parameter = std::make_unique<ParameterT>(std::forward<Args>(args)...);
    auto* ptr = parameter.get();
    apvts.createAndAddParameter(std::move(parameter));
    return ptr;
  }

  ParameterT* p;
};
}  // namespace eden::plugin::util
