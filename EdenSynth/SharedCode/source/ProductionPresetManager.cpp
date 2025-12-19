#include "ProductionPresetManager.h"
#include "PresetLoadingResult.h"
#include "PresetSavingResult.h"
#include <utility/EdenAssert.h>

namespace eden_vst {
ProductionPresetManager::ProductionPresetManager(Args&& args)
    : _presets{std::move(args.systemPresetsPath),
               std::move(args.userPresetsPath)},
      _getSerializedState{std::move(args.getSerializedState)},
      _setSerializedState{std::move(args.setSerializedState)} {}

PresetSavingResult ProductionPresetManager::saveCurrentPreset(
    const std::string& name) {
  if (_presets.containsUserPreset(name)) {
    return std::unexpected{PresetSavingError::PresetWithNameExists};
  }

  return saveOrOverwriteCurrentPreset(name);
}

PresetSavingResult ProductionPresetManager::saveOrOverwriteCurrentPreset(
    const std::string& name) {
  if (name.empty()) {
    return std::unexpected{PresetSavingError::InvalidPresetName};
  }

  const auto& newPreset = _presets.createPreset(name);
  const auto presetData = _getSerializedState();
  const auto presetFile = juce::File{newPreset.absolutePath().c_str()};
  EDEN_ASSERT(presetFile.hasWriteAccess());
  presetFile.deleteFile();
  presetFile.appendData(presetData.getData(), presetData.getSize());

  return PresetSavingSuccess::Ok;
}

PresetLoadingResult ProductionPresetManager::loadPreset(
    const eden::plugin::Preset& preset) {
  const auto& presetPath = preset.absolutePath();
  const auto presetFile = juce::File{presetPath.c_str()};

  if (!presetFile.existsAsFile()) {
    _presets.scanForPresets();
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

const eden::plugin::Presets::Container& ProductionPresetManager::presets() {
  return _presets.presets();
}
}  // namespace eden_vst
