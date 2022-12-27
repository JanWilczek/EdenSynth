///
/// \author Jan Wilczek
/// \date 02.12.2018
///
#include "FilterEnvelopeSegmentComponent.h"

FilterEnvelopeSegmentComponent::FilterEnvelopeSegmentComponent(
    AudioProcessorValueTreeState& valueTreeState,
    String labelContent,
    String parameterID)
    : _label(labelContent.toLowerCase().removeCharacters(" "), labelContent),
      _slider(Slider::SliderStyle::Rotary,
              Slider::TextEntryBoxPosition::NoTextBox) {
  _label.setJustificationType(Justification::centred);
  addAndMakeVisible(_label);

  _slider.setPopupDisplayEnabled(true, false, this);
  addAndMakeVisible(_slider);
  _attachment =
      std::make_unique<SliderAttachment>(valueTreeState, parameterID, _slider);
}

void FilterEnvelopeSegmentComponent::resized() {
  constexpr auto labelHeight = 30;
  const auto knobDim = std::min(getHeight() - labelHeight, getWidth());

  _label.setBounds(0, 0, getWidth(), labelHeight);
  _slider.setBounds(getWidth() / 2 - knobDim / 2, labelHeight, knobDim,
                    knobDim);
}
