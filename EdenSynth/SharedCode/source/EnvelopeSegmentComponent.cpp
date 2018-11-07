/// 
/// \author Jan Wilczek
/// \date 07.11.2018
/// 
#include "EnvelopeSegmentComponent.h"
#include "EnvelopeComponent.h"

EnvelopeSegmentComponent::EnvelopeSegmentComponent(EnvelopeComponent& parent, NormalisableRange<double> timeRangeMs)
	: _time(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, _curve("EnvelopeSegmentCurve")
{
	_time.setNormalisableRange(timeRangeMs);
	_time.onValueChange = [&parent]() { parent.onParameterChanged(); };
	addAndMakeVisible(_time);

	_curve.addItem("Linear", static_cast<int>(eden::EnvelopeSegmentCurve::Linear) + _comboBoxOffset);
	_curve.addItem("Exponential", static_cast<int>(eden::EnvelopeSegmentCurve::Exponential) + _comboBoxOffset);
	_curve.setSelectedId(static_cast<int>(eden::EnvelopeSegmentCurve::Exponential) + _comboBoxOffset);
	_curve.onChange = [&parent]() { parent.onParameterChanged(); };
	addAndMakeVisible(_curve);
}

void EnvelopeSegmentComponent::resized()
{
	_time.setBounds(0, 0, getWidth(), getHeight() - 30);
	_curve.setBounds(0, _time.getHeight(), getWidth(), getHeight() - _time.getHeight());
}

std::chrono::milliseconds EnvelopeSegmentComponent::getTime() const
{
	return std::chrono::milliseconds(static_cast<int>(_time.getValue()));
}

eden::EnvelopeSegmentCurve EnvelopeSegmentComponent::getCurve() const
{
	return static_cast<eden::EnvelopeSegmentCurve>(_curve.getSelectedId() - _comboBoxOffset);
}
