/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "EnvelopeComponent.h"

EnvelopeComponent::EnvelopeComponent(AudioProcessorValueTreeState& valueTreeState)
	: _attack(*this, NormalisableRange<double>(0., 10000., 10.))
	, _decay1(*this, NormalisableRange<double>(0, 4000, 10))
	, _decay2(*this, NormalisableRange<double>(0, 30000, 10))
	, _release(*this, NormalisableRange<double>(0, 40000, 10))
	, _breakLevel(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
{
	addAndMakeVisible(_attack);
	addAndMakeVisible(_decay1);
	addAndMakeVisible(_decay2);
	addAndMakeVisible(_release);
	
	_breakLevel.setNormalisableRange(NormalisableRange<double>(0., 1., 0.01));
	_breakLevel.onValueChange = [this]() { onParameterChanged(); };
	addAndMakeVisible(_breakLevel);
}

void EnvelopeComponent::paint(Graphics& g)
{
	g.setColour(Colours::purple);
	g.drawRect(getBounds(), 5);
}

void EnvelopeComponent::resized()
{
	_attack.setBounds(5, 5,getWidth() / 5 - 5, getHeight() - 10);
	_decay1.setBounds(_attack.getX() + _attack.getWidth(), _attack.getY(), _attack.getWidth(), _attack.getHeight());
	_decay2.setBounds(_decay1.getX() + _attack.getWidth(), _attack.getY(), _attack.getWidth(), _attack.getHeight());
	_release.setBounds(_decay2.getX() + _attack.getWidth(), _attack.getY(), _attack.getWidth(), _attack.getHeight());
	_breakLevel.setBounds(_release.getX() + _attack.getWidth(), _attack.getY(), _attack.getWidth(), _attack.getHeight());
}

void EnvelopeComponent::onParameterChanged()
{
	
}
