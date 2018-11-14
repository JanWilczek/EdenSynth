/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "EnvelopeComponent.h"

EnvelopeComponent::EnvelopeComponent(AudioProcessorValueTreeState& valueTreeState)
	: _attack(valueTreeState, "Attack", "envelope.adbdr.attack.time", "envelopeadbdr.attack.curve")
	, _decay1(valueTreeState, "Decay 1", "envelope.adbdr.decay1.time", "envelopeadbdr.decay1.curve")
	, _decay2(valueTreeState, "Decay 2", "envelope.adbdr.decay2.time", "envelopeadbdr.decay2.curve")
	, _release(valueTreeState, "Release", "envelope.adbdr.release.time", "envelopeadbdr.release.curve")
	, _breakLevel(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
{
	addAndMakeVisible(_attack);
	addAndMakeVisible(_decay1);
	addAndMakeVisible(_decay2);
	addAndMakeVisible(_release);
	
	_breakLevelLabel.setJustificationType(Justification::horizontallyCentred);
	addAndMakeVisible(_breakLevelLabel);
	addAndMakeVisible(_breakLevel);
	_breakLevel.setPopupDisplayEnabled(true, false, this);
	_breakLevelAttachment = std::make_unique<SliderAttachment>(valueTreeState, "envelope.adbdr.breakLevel", _breakLevel);
}

void EnvelopeComponent::paint(Graphics& g)
{
	g.setColour(getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
	g.drawRect(getBounds(), 5);
}

void EnvelopeComponent::resized()
{
	_attack.setBounds(5, 5,getWidth() / 5 - 5, getHeight() - 10);
	_decay1.setBounds(_attack.getX() + _attack.getWidth(), _attack.getY(), _attack.getWidth(), _attack.getHeight());
	_decay2.setBounds(_decay1.getX() + _attack.getWidth(), _attack.getY(), _attack.getWidth(), _attack.getHeight());
	_release.setBounds(_decay2.getX() + _attack.getWidth(), _attack.getY(), _attack.getWidth(), _attack.getHeight());
	
	_breakLevelLabel.setBounds(_release.getX() + _attack.getWidth(), _attack.getY(), _attack.getWidth(), 20);
	_breakLevel.setBounds(_release.getX() + _attack.getWidth(), _breakLevelLabel.getY() + _breakLevelLabel.getHeight(), _attack.getWidth(), _attack.getHeight() - _breakLevelLabel.getHeight());
}
