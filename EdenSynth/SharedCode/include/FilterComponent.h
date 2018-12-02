#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterControlsComponent.h"
#include "FilterEnvelopeComponent.h"

class FilterComponent : public Component
{
public:
	FilterComponent(AudioProcessorValueTreeState& valueTreeState);

	void paint(Graphics& g) override;
	void resized() override;

private:
	FilterControlsComponent _filterControls;
	FilterEnvelopeComponent _filterEnvelope;
};
