/// 
/// \author Jan Wilczek
/// \date 14.11.2018
/// 
#include "OscillatorsComponent.h"

OscillatorsComponent::OscillatorsComponent(AudioProcessorValueTreeState& valueTreeState, const eden_vst::WaveTablePathProvider& pathProvider)
	: _oscillator1(valueTreeState, "oscillator1", pathProvider)
	, _oscillator2(valueTreeState, "oscillator2", pathProvider)
	, _oscillator3(valueTreeState, "oscillator3", pathProvider)
{
	addAndMakeVisible(_oscillator1);
	addAndMakeVisible(_oscillator2);
	addAndMakeVisible(_oscillator3);
}

void OscillatorsComponent::paint(Graphics& g)
{
	g.setColour(getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
	g.drawRect(getBounds(), 5);
}

void OscillatorsComponent::resized()
{
	_oscillator1.setBounds(5, 5, getWidth() - 10, (getHeight() - 10) / 3);
	_oscillator2.setBounds(_oscillator1.getX(), _oscillator1.getY() + _oscillator1.getHeight(), _oscillator1.getWidth(), _oscillator1.getHeight());
	_oscillator3.setBounds(_oscillator1.getX(), _oscillator2.getY() + _oscillator2.getHeight(), _oscillator1.getWidth(), _oscillator1.getHeight());
}
