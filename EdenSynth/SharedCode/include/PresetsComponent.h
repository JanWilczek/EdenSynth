#pragma once

#include "JuceHeader.h"

namespace eden_vst {
class PresetManager;
}

class PresetsComponent : public juce::Component {
public:
  explicit PresetsComponent(eden_vst::PresetManager& presetManager);

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  void refreshPresetList(eden_vst::PresetManager& presetManager);

  Label _presetLabel{"presetLabel", "Preset: "};
  ComboBox _preset;
  TextButton _savePresetButton;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsComponent)
};
