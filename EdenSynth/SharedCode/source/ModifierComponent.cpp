/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "ModifierComponent.h"

ModifierComponent::ModifierComponent(AudioProcessorValueTreeState& valueTreeState)
	: _filterComponent(valueTreeState)
	, _waveshaperComponent(valueTreeState)
{
	addAndMakeVisible(_filterComponent);
	addAndMakeVisible(_waveshaperComponent);
}

void ModifierComponent::paint(Graphics& g)
{
	g.fillAll(Colours::yellow);
}

void ModifierComponent::resized()
{
	_filterComponent.setBounds(0, 0, getWidth(), 120);
	_waveshaperComponent.setBounds(0, _filterComponent.getHeight(), getWidth(), 380);
}
