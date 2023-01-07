#include "PresetManager.h"
#include "PresetSaver.h"
#include "PresetLoader.h"

namespace eden_vst {
ProductionPresetManager::ProductionPresetManager(
    const std::filesystem::path& presetsPath,
    juce::AudioProcessorValueTreeState& vts)
    : _presets{presetsPath}, _valueTreeState{vts} {}

void ProductionPresetManager::saveCurrentPreset() {
  _presetSaver = std::make_unique<PresetSaver>(_valueTreeState);
  (*_presetSaver)();
}

void ProductionPresetManager::loadPreset(const std::string& presetName) {
  PresetLoader{_valueTreeState, _presets}(presetName);
}
std::vector<std::string> ProductionPresetManager::presets() const {
  return _presets.presets();
}
}  // namespace eden_vst
