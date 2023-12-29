#include "ProductionPresetManager.h"
#include "PresetSaver.h"
#include "PresetLoader.h"

namespace eden_vst {
ProductionPresetManager::ProductionPresetManager(
    const std::filesystem::path& presetsPath,
    juce::AudioProcessorValueTreeState& vts)
    : _presets{presetsPath},
      _valueTreeState{vts},
      _presetSaver{std::make_unique<PresetSaver>(_valueTreeState)} {}

PresetSavingResult ProductionPresetManager::saveCurrentPreset(
    std::function<void(const std::string&)> onPresetAdded,
    const std::string& name) {
  return _presetSaver->saveCurrentPreset(std::move(onPresetAdded), name);
}

PresetLoadingResult ProductionPresetManager::loadPreset(
    const std::string& presetName) {
  return PresetLoader{_valueTreeState, _presets}(presetName);
}

std::vector<std::string> ProductionPresetManager::presets() const {
  return _presets.presets();
}
}  // namespace eden_vst
