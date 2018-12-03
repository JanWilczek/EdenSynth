#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.12.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterEnvelopeSegmentComponent.h"

class FilterEnvelopeComponent : public Component
{
public:
	FilterEnvelopeComponent(AudioProcessorValueTreeState&);

	void resized() override;

private:
	FilterEnvelopeSegmentComponent _attack;
	FilterEnvelopeSegmentComponent _decay;
	FilterEnvelopeSegmentComponent _sustain;
	FilterEnvelopeSegmentComponent _release;
};
