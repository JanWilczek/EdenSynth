#pragma once
#include <string>
#include <memory>
#include "Presets.h"
#include "PresetManager.h"

namespace juce {
class AudioProcessorValueTreeState;
}

namespace eden_vst {
class ProductionPresetManager : public PresetManager {
public:
  explicit ProductionPresetManager(const std::filesystem::path& presetsPath,
                                   juce::AudioProcessorValueTreeState&);
  [[nodiscard]] PresetSavingResult saveCurrentPreset(
      const std::string& name) override;
  [[nodiscard]] PresetSavingResult saveOrOverwriteCurrentPreset(
      const std::string& name) override;
  [[nodiscard]] PresetLoadingResult loadPreset(
      const std::string& presetName) override;
  [[nodiscard]] std::vector<std::string> presets() const override;

private:
  Presets _presets;
  juce::AudioProcessorValueTreeState& _valueTreeState;
  std::unique_ptr<class PresetSaver> _presetSaver;
};
}  // namespace eden_vst
