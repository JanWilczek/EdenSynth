#pragma once
#include <memory>
#include <map>
#include <string>
#include <functional>
#include "ViewModel.h"

namespace eden_vst {
class PresetManager;

namespace viewmodels {
class PresetsViewModel : public ViewModel {
public:
  using PresetList = std::map<int, std::string>;
  using PresetListChangedListener = std::function<void()>;

  explicit PresetsViewModel(PresetManager&);

  void onSavePresetClicked();
  void onSelectedPresetChanged(int selectedPresetIndex);
  void setOnPresetListChangedListener(PresetListChangedListener listener);
  int getDisplayedPresetId() const noexcept { return _displayedPresetId; }
  const PresetList& getPresetList() const noexcept { return _presetList; };

private:
  void refreshPresetList();
  void presetListChangedEvent();

  PresetManager& _presetManager;
  PresetList _presetList;
  int _displayedPresetId{0};
  PresetListChangedListener _presetListChangedListener;
};
}  // namespace viewmodels
}  // namespace eden_vst
