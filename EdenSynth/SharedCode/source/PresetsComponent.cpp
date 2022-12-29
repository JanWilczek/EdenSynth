#include "PresetsComponent.h"
#include "JuceHeader.h"

PresetsComponent::PresetsComponent() {}

PresetsComponent::~PresetsComponent() {}

void PresetsComponent::paint(juce::Graphics& g) {
  g.setColour(
      getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
  g.drawRect(getBounds(), 5);

  // placeholder text
  g.setColour(juce::Colours::white);
  g.setFont(14.0f);
  g.drawText("PresetsComponent", getLocalBounds(), juce::Justification::centred,
             true);
}

void PresetsComponent::resized() {}
