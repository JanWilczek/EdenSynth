/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "GeneratorComponent.h"

GeneratorComponent::GeneratorComponent(AudioProcessorValueTreeState& valueTreeState)
	: _envelopeComponent(valueTreeState)
{
	addAndMakeVisible(_envelopeComponent);
}

void GeneratorComponent::paint(Graphics& g)
{
	g.fillAll(Colours::darkblue);
}

void GeneratorComponent::resized()
{
	_envelopeComponent.setBounds(0, 300, getWidth(), getHeight() - 300);
}