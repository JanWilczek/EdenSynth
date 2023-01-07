#pragma once
#include <string>
#include "Presets.h"

namespace juce {
class AudioProcessorValueTreeState;
}

namespace eden_vst {
class PresetLoader {
public:
  PresetLoader(juce::AudioProcessorValueTreeState& vts, const Presets&);
  void operator()(const std::string& presetName);

private:
  void loadPreset(const std::string& presetName);

  juce::AudioProcessorValueTreeState& _pluginParameters;
  const Presets& _presets;
};
}  // namespace eden_vst
