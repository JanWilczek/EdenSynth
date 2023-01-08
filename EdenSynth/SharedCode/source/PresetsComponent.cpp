#include "PresetsComponent.h"
#include "PresetManager.h"

PresetsComponent::PresetsComponent(eden_vst::PresetManager& presetManager) {
  _presetLabel.setJustificationType(Justification::right);
  addAndMakeVisible(_presetLabel);

  refreshPresetList(presetManager);

  _preset.onChange = [this, &presetManager] {
    presetManager.loadPreset(_preset.getText().toStdString());
  };
  addAndMakeVisible(_preset);

  _savePresetButton.setButtonText("Save preset");
  _savePresetButton.onClick = [this, &presetManager] {
    presetManager.saveCurrentPreset(
        [this, &presetManager] { refreshPresetList(presetManager); });
  };
  addAndMakeVisible(_savePresetButton);
}

void PresetsComponent::refreshPresetList(
    eden_vst::PresetManager& presetManager) {
  _preset.clear(dontSendNotification);

  constexpr auto REQUIRED_FIRST_ELEMENT_ID = 1;
  std::ranges::for_each(presetManager.presets(),
                        [this, i = REQUIRED_FIRST_ELEMENT_ID](
                            const std::string& presetName) mutable {
                          _preset.addItem(presetName, i++);
                        });
}

void PresetsComponent::paint(juce::Graphics& g) {
  g.setColour(
      getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
  g.drawRect(getBounds(), 5);
}

void PresetsComponent::resized() {
  const auto PRESET_LABEL_WIDTH = getWidth() / 8;
  const auto PRESET_BOX_WIDTH = getWidth() / 2;
  const auto PRESET_BOX_HEIGHT = getHeight() / 2;
  const auto PRESET_BOX_X = (getWidth() - PRESET_BOX_WIDTH) / 2;
  const auto PRESET_BOX_Y = (getHeight() - PRESET_BOX_HEIGHT) / 2;
  const auto SAVE_PRESET_BUTTON_WIDTH = getWidth() / 8;
  const auto SAVE_PRESET_BUTTON_HEIGHT = PRESET_BOX_HEIGHT;
  const auto SAVE_PRESET_BUTTON_Y = PRESET_BOX_Y;
  constexpr auto COMPONENT_SPACING = 10;
  const auto PRESET_LABEL_X =
      PRESET_BOX_X - COMPONENT_SPACING - PRESET_LABEL_WIDTH;

  _presetLabel.setBounds(PRESET_LABEL_X, 0, PRESET_LABEL_WIDTH, getHeight());
  _preset.setBounds(PRESET_BOX_X, PRESET_BOX_Y, PRESET_BOX_WIDTH,
                    PRESET_BOX_HEIGHT);

  const auto SAVE_PRESET_BUTTON_X = _preset.getRight() + COMPONENT_SPACING;
  _savePresetButton.setBounds(SAVE_PRESET_BUTTON_X, SAVE_PRESET_BUTTON_Y,
                              SAVE_PRESET_BUTTON_WIDTH,
                              SAVE_PRESET_BUTTON_HEIGHT);
}
