#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

class ModifierComponent : public Component
{
public:
	ModifierComponent(AudioProcessorValueTreeState& valueTreeState);

	void paint(Graphics& g) override;
};
