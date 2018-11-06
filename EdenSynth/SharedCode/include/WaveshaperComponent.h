#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

class WaveshaperComponent : public Component
{
public:
	WaveshaperComponent(AudioProcessorValueTreeState& valueTreeState);

	void paint(Graphics& g) override;
};
