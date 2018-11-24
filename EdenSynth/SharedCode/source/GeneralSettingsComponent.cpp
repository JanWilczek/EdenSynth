/// 
/// \author Jan Wilczek
/// \date 21.11.2018
/// 
#include "GeneralSettingsComponent.h"

GeneralSettingsComponent::GeneralSettingsComponent(AudioProcessorValueTreeState& valueTreeState)
	: _pitchBendSemitonesUp(Slider::SliderStyle::Rotary, Slider::TextEntryBoxPosition::NoTextBox)
	, _pitchBendSemitonesDown(Slider::SliderStyle::Rotary, Slider::TextEntryBoxPosition::NoTextBox)
	, _a4Frequency(Slider::SliderStyle::Rotary, Slider::TextEntryBoxPosition::NoTextBox)
{
	_pitchBendSemitonesUpLabel.setJustificationType(Justification::horizontallyCentred);
	addAndMakeVisible(_pitchBendSemitonesUpLabel);
	_pitchBendSemitonesUp.setPopupDisplayEnabled(true, false, this);
	addAndMakeVisible(_pitchBendSemitonesUp);
	_pitchBendSemitonesUpAttachment = std::make_unique<SliderAttachment>(valueTreeState, "pitchBend.semitonesUp", _pitchBendSemitonesUp);

	_pitchBendSemitonesDownLabel.setJustificationType(Justification::horizontallyCentred);
	addAndMakeVisible(_pitchBendSemitonesDownLabel);
	_pitchBendSemitonesDown.setPopupDisplayEnabled(true, false, this);
	addAndMakeVisible(_pitchBendSemitonesDown);
	_pitchBendSemitonesDownAttachment = std::make_unique<SliderAttachment>(valueTreeState, "pitchBend.semitonesDown", _pitchBendSemitonesDown);

	_a4FrequencyLabel.setJustificationType(Justification::horizontallyCentred);
	addAndMakeVisible(_a4FrequencyLabel);
	_a4Frequency.setPopupDisplayEnabled(true, false, this);
	addAndMakeVisible(_a4Frequency);
	_a4FrequencyAttachment = std::make_unique<SliderAttachment>(valueTreeState, "frequencyOfA4", _a4Frequency);
}

void GeneralSettingsComponent::resized()
{
	constexpr int labelHeight = 20;
	constexpr int knobDim = 70;
	constexpr int componentSpace = 5;

	_pitchBendSemitonesUpLabel.setBounds(5, 5, getWidth() - 10, labelHeight);
	_pitchBendSemitonesUp.setBounds(getWidth() / 2 - knobDim / 2, _pitchBendSemitonesUpLabel.getY() + _pitchBendSemitonesUpLabel.getHeight(), knobDim, knobDim);

	_pitchBendSemitonesDownLabel.setBounds(_pitchBendSemitonesUpLabel.getX(), _pitchBendSemitonesUp.getY() + _pitchBendSemitonesUp.getHeight() + componentSpace, _pitchBendSemitonesUpLabel.getWidth(), labelHeight);
	_pitchBendSemitonesDown.setBounds(_pitchBendSemitonesUp.getX(), _pitchBendSemitonesDownLabel.getY() + _pitchBendSemitonesDownLabel.getHeight(), knobDim, knobDim);

	_a4FrequencyLabel.setBounds(_pitchBendSemitonesUpLabel.getX(), _pitchBendSemitonesDown.getY() + _pitchBendSemitonesDown.getHeight() + componentSpace, _pitchBendSemitonesUpLabel.getWidth(), labelHeight);
	_a4Frequency.setBounds(_pitchBendSemitonesUp.getX(), _a4FrequencyLabel.getY() + _a4FrequencyLabel.getHeight(), knobDim, knobDim);
}

void GeneralSettingsComponent::paint(Graphics& g)
{
	g.setColour(getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
	g.drawRect(getBounds(), 5);
}
