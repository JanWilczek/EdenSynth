#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

class FilterComponent : public Component
{
public:
	FilterComponent(AudioProcessorValueTreeState& valueTreeState);

	void paint(Graphics& g) override;
};