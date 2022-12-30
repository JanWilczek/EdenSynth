#pragma once

#include "JuceHeader.h"
#include "Presets.h"

class PresetsComponent : public juce::Component {
public:
  explicit PresetsComponent(
      eden_vst::Presets,
      std::function<void(const std::string&)> loadPresetAction,
      std::function<void()> savePresetAction);

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  Label _presetLabel{"presetLabel", "Preset: "};
  ComboBox _preset;
  TextButton _savePresetButton;
  eden_vst::Presets _presets;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsComponent)
};
