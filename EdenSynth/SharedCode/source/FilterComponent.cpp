/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "FilterComponent.h"
#include <memory>

FilterComponent::FilterComponent(AudioProcessorValueTreeState& valueTreeState)
	: _cutoffLabel("cutoffLabel", "Cutoff")
	, _cutoff(Slider::SliderStyle::Rotary, Slider::TextEntryBoxPosition::NoTextBox)
	, _resonanceLabel("resonanceLabel", "Resonance")
	, _resonance(Slider::SliderStyle::Rotary, Slider::TextEntryBoxPosition::NoTextBox)
{
	_cutoffLabel.setJustificationType(Justification::horizontallyCentred);
	addAndMakeVisible(_cutoffLabel);
	addAndMakeVisible(_cutoff);
	_cutoffAttachment = std::make_unique<SliderAttachment>(valueTreeState, "filter.cutoff", _cutoff);

	_resonanceLabel.setJustificationType(Justification::horizontallyCentred);
	addAndMakeVisible(_resonanceLabel);
	addAndMakeVisible(_resonance);
	_resonanceAttachment = std::make_unique<SliderAttachment>(valueTreeState, "filter.resonance", _resonance);
}

void FilterComponent::paint(Graphics& g)
{
	g.setColour(getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
	g.drawRect(getBounds(), 5);
}

void FilterComponent::resized()
{
	_cutoffLabel.setBounds(5, 5, 80, 20);
	_cutoff.setBounds(5, 25, 80, 80);

	_resonanceLabel.setBounds(150, 5, 80, 20);
	_resonance.setBounds(150, 25, 80, 80);
}
