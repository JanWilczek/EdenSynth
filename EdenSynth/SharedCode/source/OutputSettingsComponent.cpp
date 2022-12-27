///
/// \author Jan Wilczek
/// \date 21.11.2018
///
#include "OutputSettingsComponent.h"

OutputSettingsComponent::OutputSettingsComponent(
    AudioProcessorValueTreeState& valueTreeState)
    : _volume(Slider::SliderStyle::Rotary,
              Slider::TextEntryBoxPosition::NoTextBox) {
  _volumeLabel.setJustificationType(Justification::horizontallyCentred);
  addAndMakeVisible(_volumeLabel);

  _volume.setPopupDisplayEnabled(true, false, this);
  addAndMakeVisible(_volume);

  _volumeAttachment = std::make_unique<SliderAttachment>(
      valueTreeState, "output.volume", _volume);
}

void OutputSettingsComponent::resized() {
  constexpr int knobDim = 70;

  _volumeLabel.setBounds(5, 5, getWidth(), 20);
  _volume.setBounds(getWidth() / 2 - knobDim / 2,
                    _volumeLabel.getY() + _volumeLabel.getHeight(), knobDim,
                    knobDim);
}

void OutputSettingsComponent::paint(Graphics& g) {
  g.setColour(
      getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
  g.drawRect(0, 0, getWidth(), getHeight(),
             5);  // Some components need their global bounds as a rectangle for
                  // others it should be relative to their own position.
}
