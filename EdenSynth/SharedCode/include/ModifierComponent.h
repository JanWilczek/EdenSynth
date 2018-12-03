#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterComponent.h"
#include "WaveshaperComponent.h"

class ModifierComponent : public Component
{
public:
	ModifierComponent(AudioProcessorValueTreeState& valueTreeState);

	void resized() override;

private:
	FilterComponent _filterComponent;
	WaveshaperComponent _waveshaperComponent;
};
