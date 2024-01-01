#pragma once

#include "JuceHeader.h"
#include "ErrorDialogListener.h"
#include "Visibility.h"

namespace eden_vst::viewmodels {
class PresetsViewModel;
}

class PresetsComponent : public juce::Component,
                         public eden_vst::ErrorDialogListener {
public:
  explicit PresetsComponent(
      std::unique_ptr<eden_vst::viewmodels::PresetsViewModel> presetsViewModel);

  void paint(juce::Graphics&) override;
  void resized() override;

  void showErrorDialogWithMessage(const std::string& message) override;

private:
  void refreshPresetList();
  void onPresetNameInputDialogVisibilityChanged(
      eden_vst::Visibility visibility);
  void showPresetNameInputDialog();
  void hidePresetNameInputDialog();

  std::unique_ptr<eden_vst::viewmodels::PresetsViewModel> _viewModel;

  Label _presetLabel{"presetLabel", "Preset: "};
  ComboBox _preset;
  TextButton _savePresetButton;
  std::unique_ptr<AlertWindow> _presetNameInputDialog;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsComponent)
};
