#include "ProductionPresetManager.h"
#include "PresetLoadingResult.h"
#include "PresetSavingResult.h"
#include <utility/EdenAssert.h>

namespace eden::plugin {
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
  juce::FileOutputStream outputStream{presetFile};
  if (outputStream.openedOk()) {
    outputStream.setPosition(0);
    outputStream.truncate();
    juce::JSON::writeToStream(outputStream, presetData.toVar(),
                              juce::JSON::FormatOptions{}
                                  .withIndentLevel(2)
                                  .withMaxDecimalPlaces(2)
                                  .withSpacing(juce::JSON::Spacing::multiLine));
    return PresetSavingSuccess::Ok;
  }
  return std::unexpected{PresetSavingError::FailedToWritePresetFile};
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

  juce::FileInputStream inputStream{presetFile};
  if (!inputStream.openedOk()) {
    return std::unexpected{PresetLoadingError::FailedToReadFile};
  }
  const auto presetData = juce::JSON::parse(inputStream);
  // TODO: Validate correctness

  const auto parameters = eden::plugin::Parameters::fromChecked(presetData);
  EDEN_ASSERT(parameters.has_value());
  _setSerializedState(parameters.value());

  return PresetLoadingSuccess::Ok;
}

const eden::plugin::Presets::Container& ProductionPresetManager::presets() {
  return _presets.presets();
}
}  // namespace eden::plugin
