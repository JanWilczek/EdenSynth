#pragma once

#include "JuceHeader.h"
#include "ErrorDialogListener.h"
#include "Visibility.h"

namespace eden::plugin::viewmodels {
class PresetsViewModel;
}

class PresetsComponent : public juce::Component,
                         public eden::plugin::ErrorDialogListener {
public:
  explicit PresetsComponent(
      std::unique_ptr<eden::plugin::viewmodels::PresetsViewModel> presetsViewModel);

  void paint(juce::Graphics&) override;
  void resized() override;

  void showErrorDialogWithMessage(const std::string& message) override;

private:
  void refreshPresetList();
  void onPresetNameInputDialogVisibilityChanged(
      eden::plugin::Visibility visibility);
  void showPresetNameInputDialog();
  void hidePresetNameInputDialog();

  std::unique_ptr<eden::plugin::viewmodels::PresetsViewModel> _viewModel;

  Label _presetLabel{"presetLabel", "Preset: "};
  ComboBox _preset;
  TextButton _savePresetButton;
  std::unique_ptr<AlertWindow> _presetNameInputDialog;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsComponent)
};
