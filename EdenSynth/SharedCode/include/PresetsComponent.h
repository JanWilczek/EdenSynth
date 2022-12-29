#pragma once

#include "JuceHeader.h"

class PresetsComponent : public juce::Component {
public:
  explicit PresetsComponent(std::function<void()>&& savePresetAction);

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  Label _presetLabel{"presetLabel", "Preset: "};
  ComboBox _preset;
  TextButton _savePresetButton;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsComponent)
};
