#pragma once
#include <filesystem>
#include "PresetSavingResult.h"

namespace juce {
class AudioProcessorValueTreeState;
}  // namespace juce

namespace eden_vst {
class PresetSaver {
public:
  explicit PresetSaver(juce::AudioProcessorValueTreeState&);
  [[nodiscard]] PresetSavingResult saveOrOverwriteCurrentPreset(
      const std::filesystem::path& outputPath);

private:
  juce::AudioProcessorValueTreeState& _pluginParameters;
};
}  // namespace eden_vst
