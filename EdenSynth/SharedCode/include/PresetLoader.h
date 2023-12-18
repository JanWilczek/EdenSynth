#pragma once
#include <string>
#include "PresetLoadingResult.h"
#include "Presets.h"

namespace juce {
class AudioProcessorValueTreeState;
}

namespace eden_vst {
class PresetLoader {
public:
  PresetLoader(juce::AudioProcessorValueTreeState& vts, const Presets&);
  [[nodiscard]] PresetLoadingResult operator()(const std::string& presetName);

private:
  [[nodiscard]] PresetLoadingResult loadPreset(const std::string& presetName);

  juce::AudioProcessorValueTreeState& _pluginParameters;
  const Presets& _presets;
};
}  // namespace eden_vst
