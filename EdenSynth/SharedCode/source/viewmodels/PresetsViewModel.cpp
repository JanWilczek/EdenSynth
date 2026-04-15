#include "viewmodels/PresetsViewModel.h"
#include "PresetLoadingResult.h"
#include "PresetManager.h"
#include <algorithm>
#include <iterator>
#include <tuple>
#include <utility/EdenAssert.h>

namespace eden::plugin::viewmodels {
PresetsViewModel::PresetsViewModel(PresetManager& presetManager)
    : _presetManager{presetManager} {
  refreshPresetList();
}

void PresetsViewModel::onSavePresetClicked() {
  _onPresetNameInputDialogVisibilityChangedHandler(Visibility::Visible);
}

void PresetsViewModel::onPresetNameGiven(const std::string& presetName) {
  const auto result = _presetManager.saveCurrentPreset(presetName);
  handleSavingResult(result, presetName);
  _onPresetNameInputDialogVisibilityChangedHandler(Visibility::Gone);
}

void PresetsViewModel::onSelectedPresetChanged(int selectedPresetIndex) {
  _displayedPresetId = selectedPresetIndex;
  const auto& preset = _presetList.at(getDisplayedPresetId());
  const auto result = _presetManager.loadPreset(preset);
  handleLoadingResult(result);
}

void PresetsViewModel::setOnPresetListChangedListener(
    PresetListChangedListener listener) {
  _presetListChangedListener = std::move(listener);
}

void PresetsViewModel::onOverwritePresetClicked(
    const std::string& presetToOverwriteName) {
  const auto result =
      _presetManager.saveOrOverwriteCurrentPreset(presetToOverwriteName);
  handleSavingResult(result, presetToOverwriteName);
}

void PresetsViewModel::handleSavingResult(PresetSavingResult result,
                                          const std::string& presetName) {
  if (result.has_value()) {
    refreshPresetList();
    if (const auto it = std::ranges::find_if(_presetList,
                                             [&presetName](const auto& entry) {
                                               return entry.second.name() ==
                                                      presetName;
                                             });
        it != _presetList.end()) {
      _displayedPresetId = it->first;
      presetListChangedEvent();
    } else {
      EDEN_ASSERT(false);
    }
  } else {
    switch (result.error()) {
      case PresetSavingError::InvalidPresetName:
        showErrorDialogWithMessage("Invalid preset name.");
        break;
      case PresetSavingError::PresetWithNameExists:
        _onShouldOverridePresetDialogVisibilityChangedHandler(
            Visibility::Visible, presetName);
        break;
      case PresetSavingError::FailedToWritePresetFile:
        showErrorDialogWithMessage("Failed to save the preset file.");
        break;
      case PresetSavingError::FailedToCreatePresetFile:
        showErrorDialogWithMessage("Failed to create the preset file.");
        break;
    }
  }
}

void PresetsViewModel::refreshPresetList() {
  _presetList.clear();
  constexpr auto requiredFirstElementId = 1;
  std::ranges::for_each(
      _presetManager.presets(),
      [this, i = requiredFirstElementId](const auto& preset) mutable {
        _presetList.insert(std::make_pair(i, preset));
        i++;
      });
}

void PresetsViewModel::presetListChangedEvent() {
  _presetListChangedListener();
}

void PresetsViewModel::handleLoadingResult(PresetLoadingResult result) {
  if (result) {
    return;
  }

  using enum PresetLoadingError;

  switch (result.error()) {
    case DoesNotExist:
      showErrorDialogWithMessage(
          "Failed to load preset: preset does not exist.");
      break;
    case FailedToReadFile:
      showErrorDialogWithMessage("Failed to read the preset file");
      break;
    case NoPermission:
      showErrorDialogWithMessage(
          "Failed to load preset: no permission to read the preset file");
      break;
  }
}

void PresetsViewModel::showErrorDialogWithMessage(std::string message) {
  _errorDialogListeners.call(
      [message = std::move(message)](ErrorDialogListener& l) {
        l.showErrorDialogWithMessage(message);
      });
}

auto PresetsViewModel::getPresetList() const noexcept -> PresetNameList {
  PresetNameList result;
  std::ranges::for_each(_presetList, [&](const auto& idAndPreset) {
    result[idAndPreset.first] = idAndPreset.second.name();
  });
  return result;
}

void PresetsViewModel::addErrorDialogListener(ErrorDialogListener* listener) {
  _errorDialogListeners.add(listener);
}

void PresetsViewModel::onPresetNameInputDialogCancelClicked() {
  _onPresetNameInputDialogVisibilityChangedHandler(Visibility::Gone);
}
}  // namespace eden::plugin::viewmodels
