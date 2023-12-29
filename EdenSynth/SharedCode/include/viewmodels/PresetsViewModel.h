#pragma once
#include <memory>
#include <map>
#include <string>
#include <functional>
#include "ViewModel.h"
#include "ErrorDialogListener.h"
#include "PresetLoadingResult.h"
#include "JuceHeader.h"
#include "Visibility.h"

namespace eden_vst {
class PresetManager;

namespace viewmodels {
class PresetsViewModel : public ViewModel {
public:
  using PresetList = std::map<int, std::string>;
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
  const PresetList& getPresetList() const noexcept { return _presetList; };
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
