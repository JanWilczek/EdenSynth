#pragma once
/// 
/// \author Jan Wilczek
/// \date 21.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

class OutputSettingsComponent : public Component
{
public:
	using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

	OutputSettingsComponent(AudioProcessorValueTreeState&);

	void resized() override;
	void paint(Graphics& g) override;

private:
	Label _volumeLabel{ "globalVolumeLabel", "Volume" };
	Slider _volume;
	std::unique_ptr<SliderAttachment> _volumeAttachment;
};
