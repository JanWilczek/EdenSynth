/// 
/// \author Jan Wilczek
/// \date 07.11.2018
/// 
#include "EnvelopeSegmentComponent.h"
#include "EnvelopeComponent.h"
#include <string>

EnvelopeSegmentComponent::EnvelopeSegmentComponent(AudioProcessorValueTreeState& valueTreeState, String timeParameterID, String curveParameterID)
	: _time(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, _curve("EnvelopeSegmentCurve")
{
	_time.onValueChange = [this]() { _time.setTooltip(std::to_string(_time.getValue()) + " ms"); };
	addAndMakeVisible(_time);
	_timeAttachment = std::make_unique<SliderAttachment>(valueTreeState, timeParameterID, _time);

	_curve.addItem("Linear", static_cast<int>(eden::EnvelopeSegmentCurve::Linear));
	_curve.addItem("Exponential", static_cast<int>(eden::EnvelopeSegmentCurve::Exponential));
	_curve.setSelectedId(static_cast<int>(eden::EnvelopeSegmentCurve::Exponential));
	addAndMakeVisible(_curve);
	_curveAttachment = std::make_unique<ComboBoxAttachment>(valueTreeState, curveParameterID, _curve);
}

void EnvelopeSegmentComponent::resized()
{
	_time.setBounds(0, 0, getWidth(), getHeight() - 30);
	_curve.setBounds(0, _time.getHeight(), getWidth(), getHeight() - _time.getHeight());
}
