#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

class FilterComponent : public Component
{
public:
	using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

	FilterComponent(AudioProcessorValueTreeState& valueTreeState);

	void paint(Graphics& g) override;

private:
	Label _cutoffLabel;
	Slider _cutoff;
	std::unique_ptr<SliderAttachment> _cutoffAttachment;

	Label _resonanceLabel;
	Slider _resonance;
	std::unique_ptr<SliderAttachment> _resonanceAttachment;
};
