#pragma once
#include <string>
#include <filesystem>
#include <functional>
#include "Presets.h"
#include "PresetManager.h"
#include "PresetLoadingResult.h"

namespace juce {
class AudioProcessorValueTreeState;
}

namespace eden_vst {
class ProductionPresetManager : public PresetManager {
public:
  explicit ProductionPresetManager(const std::filesystem::path& presetsPath,
                                   juce::AudioProcessorValueTreeState&);
  void saveCurrentPreset(
      std::function<void(const std::string&)> onPresetAdded) override;
  [[nodiscard]] PresetLoadingResult loadPreset(
      const std::string& presetName) override;
  std::vector<std::string> presets() const override;

private:
  Presets _presets;
  juce::AudioProcessorValueTreeState& _valueTreeState;
  std::unique_ptr<class PresetSaver> _presetSaver;
};
}  // namespace eden_vst
