#pragma once

#include "JuceHeader.h"

namespace eden_vst::viewmodels {
class PresetsViewModel;
}

class PresetsComponent : public juce::Component {
public:
  explicit PresetsComponent(
      std::unique_ptr<eden_vst::viewmodels::PresetsViewModel> presetsViewModel);

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  void refreshPresetList();

  std::unique_ptr<eden_vst::viewmodels::PresetsViewModel> _viewModel;

  Label _presetLabel{"presetLabel", "Preset: "};
  ComboBox _preset;
  TextButton _savePresetButton;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsComponent)
};
