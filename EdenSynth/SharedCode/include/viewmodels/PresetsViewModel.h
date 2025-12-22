#pragma once
#include <memory>
#include <map>
#include <string>
#include <functional>
#include "ViewModel.h"
#include "ErrorDialogListener.h"
#include "PresetLoadingResult.h"
#include "PresetSavingResult.h"
#include "JuceHeader.h"
#include "Visibility.h"
#include "../source/presets/Preset.h"

namespace eden_vst {
class PresetManager;

namespace viewmodels {
/// <summary>
/// ViewModel transforming preset-related Model data
/// into View data and vice-versa.
/// </summary>
class PresetsViewModel : public ViewModel {
public:
  using PresetNameList = std::map<int, std::string>;
  using PresetList = std::map<int, eden::plugin::Preset>;
  using PresetListChangedListener = std::function<void()>;
  using OnShouldOverridePresetDialogVisibilityChanged =
      std::function<void(Visibility, const std::string&)>;
  using OnPresetNameInputDialogVisibilityChanged =
      std::function<void(Visibility)>;

  explicit PresetsViewModel(PresetManager&);

  void onSavePresetClicked();
  void onPresetNameGiven(const std::string& presetName);
  void onSelectedPresetChanged(int selectedPresetIndex);
  void setOnPresetListChangedListener(PresetListChangedListener listener);
  int getDisplayedPresetId() const noexcept { return _displayedPresetId; }
  PresetNameList getPresetList() const noexcept;
  void addErrorDialogListener(ErrorDialogListener*);
  void onShouldOverridePresetDialogVisibilityChanged(
      OnShouldOverridePresetDialogVisibilityChanged handler) {
    _onShouldOverridePresetDialogVisibilityChangedHandler = std::move(handler);
  }
  void onPresetNameInputDialogVisibilityChanged(
      OnPresetNameInputDialogVisibilityChanged handler) {
    _onPresetNameInputDialogVisibilityChangedHandler = std::move(handler);
  }
  void onPresetNameInputDialogCancelClicked();
  void onOverwritePresetClicked(const std::string& presetToOverwriteName);

private:
  void refreshPresetList();
  void presetListChangedEvent();
  void handleLoadingResult(PresetLoadingResult result);
  void handleSavingResult(PresetSavingResult result,
                          const std::string& presetName);
  void showErrorDialogWithMessage(std::string message);

  PresetManager& _presetManager;
  PresetList _presetList;
  int _displayedPresetId{0};
  PresetListChangedListener _presetListChangedListener;
  juce::ListenerList<ErrorDialogListener> _errorDialogListeners;
  OnShouldOverridePresetDialogVisibilityChanged
      _onShouldOverridePresetDialogVisibilityChangedHandler;
  OnPresetNameInputDialogVisibilityChanged
      _onPresetNameInputDialogVisibilityChangedHandler;
};
}  // namespace viewmodels
}  // namespace eden_vst
