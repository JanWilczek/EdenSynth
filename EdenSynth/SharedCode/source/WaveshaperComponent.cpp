/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "WaveshaperComponent.h"

WaveshaperComponent::WaveshaperComponent(AudioProcessorValueTreeState& valueTreeState)
{
	addAndMakeVisible(_canvas);
}

void WaveshaperComponent::paint(Graphics& g)
{
	g.setColour(getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
	g.drawRect(0,0,getWidth(),getHeight(), 5);
}

void WaveshaperComponent::resized()
{
	_canvas.setBounds(10, 10, getHeight() - 20, getHeight() - 20);
}
