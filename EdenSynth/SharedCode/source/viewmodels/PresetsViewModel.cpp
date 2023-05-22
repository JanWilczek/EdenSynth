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
  _presetManager.saveCurrentPreset([this](const std::string& addedPresetName) {
    refreshPresetList();
    if (const auto it =
            std::ranges::find_if(_presetList,
                                 [&addedPresetName](const auto& entry) {
                                   return entry.second == addedPresetName;
                                 });
        it != _presetList.end()) {
      _displayedPresetId = it->first;
      presetListChangedEvent();
    } else {
      assert(false);
    }
  });
}

void PresetsViewModel::onSelectedPresetChanged(int selectedPresetIndex) {
  _displayedPresetId = selectedPresetIndex;
  const auto& presetName = _presetList.at(getDisplayedPresetId());
  _presetManager.loadPreset(presetName);
}

void PresetsViewModel::setOnPresetListChangedListener(
    PresetListChangedListener listener) {
  _presetListChangedListener = std::move(listener);
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
}  // namespace eden_vst::viewmodels
