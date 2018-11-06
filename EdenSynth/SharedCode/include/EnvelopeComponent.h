#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

class EnvelopeComponent : public Component
{
public:
	EnvelopeComponent(AudioProcessorValueTreeState& valueTreeState);

	void paint(Graphics& g) override;
};
