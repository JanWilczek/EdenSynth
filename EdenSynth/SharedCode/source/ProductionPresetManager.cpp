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

void ProductionPresetManager::saveCurrentPreset(
    std::function<void(const std::string&)> onPresetAdded) {
  (*_presetSaver)(std::move(onPresetAdded));
}

void ProductionPresetManager::loadPreset(const std::string& presetName) {
  const auto result = PresetLoader{_valueTreeState, _presets}(presetName);
  handleLoadingResult(result);
}

void ProductionPresetManager::handleLoadingResult(
    std::expected<PresetLoader::LoadingResult, PresetLoader::LoadingError>
        result) {
  if (result) {
    return;
  }

  using enum PresetLoader::LoadingError;

  switch (result.error()) {
    case DoesNotExist:
      break;
    case WrongTag:
      break;
  }
}

std::vector<std::string> ProductionPresetManager::presets() const {
  return _presets.presets();
}
}  // namespace eden_vst
