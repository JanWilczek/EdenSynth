#pragma once
#include <string>
#include <filesystem>
#include "Presets.h"

namespace juce {
class AudioProcessorValueTreeState;
}

namespace eden_vst {
class PresetManager {
public:
  virtual ~PresetManager();
  virtual void saveCurrentPreset() = 0;
  virtual void loadPreset(const std::string& presetName) = 0;
  virtual std::vector<std::string> presets() const = 0;
};

inline PresetManager::~PresetManager() {}

class ProductionPresetManager : public PresetManager {
public:
  explicit ProductionPresetManager(const std::filesystem::path& presetsPath,
                                   juce::AudioProcessorValueTreeState&);
  void saveCurrentPreset() override;
  void loadPreset(const std::string& presetName) override;
  std::vector<std::string> presets() const override;

private:
  Presets _presets;
  juce::AudioProcessorValueTreeState& _valueTreeState;
  std::unique_ptr<class PresetSaver> _presetSaver;
};
}  // namespace eden_vst
