#pragma once
#include <string>
#include <expected>
#include "Presets.h"

namespace juce {
class AudioProcessorValueTreeState;
}

namespace eden_vst {
class PresetLoader {
public:
  enum class LoadingResult { Ok };
  enum class LoadingError { DoesNotExist, WrongTag };

  PresetLoader(juce::AudioProcessorValueTreeState& vts, const Presets&);
  std::expected<LoadingResult, LoadingError> operator()(
      const std::string& presetName);

private:
  std::expected<LoadingResult, LoadingError> loadPreset(
      const std::string& presetName);

  juce::AudioProcessorValueTreeState& _pluginParameters;
  const Presets& _presets;
};
}  // namespace eden_vst
