/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "FilterComponent.h"

FilterComponent::FilterComponent(AudioProcessorValueTreeState& valueTreeState)
	: _filterControls(valueTreeState)
	, _filterEnvelope(valueTreeState)
{
	addAndMakeVisible(_filterControls);
	addAndMakeVisible(_filterEnvelope);
}

void FilterComponent::paint(Graphics& g)
{
	g.setColour(getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
	g.drawRect(getBounds(), 5);
}

void FilterComponent::resized()
{
	_filterControls.setBounds(5, 5, getWidth() - 10, (getHeight() - 10) / 2);
	_filterEnvelope.setBounds(_filterControls.getX(), _filterControls.getY() + _filterControls.getHeight(), _filterControls.getWidth(), _filterControls.getHeight());
}
