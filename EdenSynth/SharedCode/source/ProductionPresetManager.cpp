#include "ProductionPresetManager.h"
#include "PresetLoadingResult.h"
#include "PresetSavingResult.h"

namespace eden_vst {
ProductionPresetManager::ProductionPresetManager(Args&& args)
    : _presets{std::move(args.userPresetsPath)},
      _getSerializedState{std::move(args.getSerializedState)},
      _setSerializedState{std::move(args.setSerializedState)} {}

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

  const auto presetData = _getSerializedState();
  const auto presetFile = juce::File{presetOutputPath.c_str()};
  jassert(presetFile.hasWriteAccess());
  presetFile.deleteFile();
  presetFile.appendData(presetData.getData(), presetData.getSize());

  return PresetSavingSuccess::Ok;
}

PresetLoadingResult ProductionPresetManager::loadPreset(
    const std::string& presetName) {
  // check if the preset exists
  if (_presets.notContains(presetName)) {
    return std::unexpected{PresetLoadingError::DoesNotExist};
  }

  // if yes, load from file
  const auto presetPath = _presets.pathToExistingPreset(presetName);
  const auto presetFile = juce::File{presetPath.c_str()};

  if (!presetFile.existsAsFile()) {
    return std::unexpected{PresetLoadingError::DoesNotExist};
  }

  if (!presetFile.hasReadAccess()) {
    return std::unexpected{PresetLoadingError::NoPermission};
  }

  juce::MemoryBlock presetData;
  const auto result = presetFile.loadFileAsData(presetData);

  if (!result) {
    return std::unexpected{PresetLoadingError::FailedToReadFile};
  }

  _setSerializedState(presetData);

  return PresetLoadingSuccess::Ok;
}

std::vector<std::string> ProductionPresetManager::presets() const {
  return _presets.presets();
}
}  // namespace eden_vst
