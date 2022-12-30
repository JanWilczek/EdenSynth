#pragma once
#include <string>
#include "Presets.h"

namespace juce {
class AudioProcessorValueTreeState;
}

namespace eden_vst {
class PresetLoader {
public:
  explicit PresetLoader(juce::AudioProcessorValueTreeState& vts, Presets);
  void operator()(const std::string& presetName);

private:
  void loadPreset(const std::string& presetName);

  juce::AudioProcessorValueTreeState& _pluginParameters;
  Presets _presets;
};
}  // namespace eden_vst
