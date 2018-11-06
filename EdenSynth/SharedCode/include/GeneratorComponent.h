#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

class GeneratorComponent : public Component
{
public:
	GeneratorComponent(AudioProcessorValueTreeState& valueTreeState);

	void paint(Graphics& g) override;
};
