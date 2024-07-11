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
    const std::string& name) {
  if (_presets.contains(name)) {
    return std::unexpected{PresetSavingError::PresetWithNameExists};
  }

  return saveOrOverwriteCurrentPreset(name);
}

PresetSavingResult ProductionPresetManager::saveOrOverwriteCurrentPreset(
    const std::string& name) {
  if (name.empty()) {
    return std::unexpected{PresetSavingError::InvalidPresetName};
  }

  const auto presetOutputPath = _presets.pathToPreset(name);
  if (presetOutputPath.empty()) {
    return std::unexpected{PresetSavingError::InvalidPresetName};
  }

  return _presetSaver->saveOrOverwriteCurrentPreset(presetOutputPath);
}

PresetLoadingResult ProductionPresetManager::loadPreset(
    const std::string& presetName) {
  return PresetLoader{_valueTreeState, _presets}(presetName);
}

std::vector<std::string> ProductionPresetManager::presets() const {
  return _presets.presets();
}
}  // namespace eden_vst
