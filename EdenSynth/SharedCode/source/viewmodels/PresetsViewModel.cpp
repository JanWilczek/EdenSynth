#include "viewmodels/PresetsViewModel.h"
#include "PresetManager.h"
#include <ranges>
#include <algorithm>
#include <cassert>

namespace eden_vst::viewmodels {
PresetsViewModel::PresetsViewModel(PresetManager& presetManager)
    : _presetManager{presetManager} {
  refreshPresetList();
}

void PresetsViewModel::onSavePresetClicked() {
  _onPresetNameInputDialogVisibilityChangedHandler(Visibility::Visible);
}

void PresetsViewModel::onPresetNameGiven(const std::string& presetName) {
  const auto result =
      _presetManager.saveCurrentPreset([](const std::string&) {}, presetName);

  if (result.has_value()) {
    refreshPresetList();
    if (const auto it = std::ranges::find_if(_presetList,
                                             [&presetName](const auto& entry) {
                                               return entry.second ==
                                                      presetName;
                                             });
        it != _presetList.end()) {
      _displayedPresetId = it->first;
      presetListChangedEvent();
    } else {
      assert(false);
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
  _onPresetNameInputDialogVisibilityChangedHandler(Visibility::Gone);
}

void PresetsViewModel::onSelectedPresetChanged(int selectedPresetIndex) {
  _displayedPresetId = selectedPresetIndex;
  const auto& presetName = _presetList.at(getDisplayedPresetId());
  const auto result = _presetManager.loadPreset(presetName);
  handleLoadingResult(result);
}

void PresetsViewModel::setOnPresetListChangedListener(
    PresetListChangedListener listener) {
  _presetListChangedListener = std::move(listener);
}

void PresetsViewModel::onOverwritePresetClicked(
    const std::string& presetToOverwriteName) {
  // TODO!
  onPresetNameGiven(presetToOverwriteName);
}

void PresetsViewModel::refreshPresetList() {
  _presetList.clear();
  constexpr auto REQUIRED_FIRST_ELEMENT_ID = 1;
  std::ranges::for_each(_presetManager.presets(),
                        [this, i = REQUIRED_FIRST_ELEMENT_ID](
                            const std::string& presetName) mutable {
                          _presetList[i++] = presetName;
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
    case WrongTag:
      showErrorDialogWithMessage(
          "Failed to load preset: preset does not come from the plugin.");
      break;
  }
}

void PresetsViewModel::showErrorDialogWithMessage(std::string message) {
  _errorDialogListeners.call(
      [message = std::move(message)](ErrorDialogListener& l) {
        l.showErrorDialogWithMessage(message);
      });
}

void PresetsViewModel::addErrorDialogListener(ErrorDialogListener* listener) {
  _errorDialogListeners.add(listener);
}

void PresetsViewModel::onPresetNameInputDialogCancelClicked() {
  _onPresetNameInputDialogVisibilityChangedHandler(Visibility::Gone);
}
}  // namespace eden_vst::viewmodels
