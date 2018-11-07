#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeSegmentComponent.h"

class EnvelopeComponent : public Component
{
public:
	EnvelopeComponent(AudioProcessorValueTreeState& valueTreeState);

	void paint(Graphics& g) override;
	void resized() override;
	void onParameterChanged();

private:
	EnvelopeSegmentComponent _attack;
	EnvelopeSegmentComponent _decay1;
	EnvelopeSegmentComponent _decay2;
	EnvelopeSegmentComponent _release;
	Slider _breakLevel;
};
