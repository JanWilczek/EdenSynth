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

void ModifierComponent::resized()
{
	_filterComponent.setBounds(0, 0, getWidth(), 200);
	_waveshaperComponent.setBounds(0, _filterComponent.getHeight(), getWidth(), getHeight() - _filterComponent.getHeight());
}
