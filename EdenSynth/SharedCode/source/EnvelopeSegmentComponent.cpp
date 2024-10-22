///
/// \author Jan Wilczek
/// \date 07.11.2018
///
#include "EnvelopeSegmentComponent.h"
#include "EnvelopeComponent.h"

EnvelopeSegmentComponent::EnvelopeSegmentComponent(
    AudioProcessorValueTreeState& valueTreeState,
    String segmentLabel,
    String timeParameterID,
    String curveParameterID)
    : _segmentName(segmentLabel.toLowerCase().removeCharacters(" "),
                   segmentLabel),
      _time(Slider::SliderStyle::LinearVertical,
            Slider::TextEntryBoxPosition::NoTextBox),
      _curve("EnvelopeSegmentCurve") {
  _segmentName.setJustificationType(Justification::horizontallyCentred);
  addAndMakeVisible(_segmentName);

  _time.setPopupDisplayEnabled(true, false, this);
  addAndMakeVisible(_time);
  _timeAttachment = std::make_unique<SliderAttachment>(valueTreeState,
                                                       timeParameterID, _time);

  _curve.addItem("Linear", 1);
  _curve.addItem("Exponential", 2);
  addAndMakeVisible(_curve);
  _curveAttachment = std::make_unique<ComboBoxAttachment>(
      valueTreeState, curveParameterID, _curve);
}

void EnvelopeSegmentComponent::resized() {
  _segmentName.setBounds(0, 0, getWidth(), 20);
  _time.setBounds(0, _segmentName.getHeight(), getWidth(),
                  getHeight() - _segmentName.getHeight() - 30);
  _curve.setBounds(0, _time.getY() + _time.getHeight(), getWidth(),
                   getHeight() - _segmentName.getHeight() - _time.getHeight());
}
