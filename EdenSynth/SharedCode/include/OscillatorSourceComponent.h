#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.02.2019
/// 
#include "../JuceLibraryCode/JuceHeader.h"

namespace eden_vst
{
	class WaveTablePathProvider;
}

class OscillatorSourceComponent : public Component, public Button::Listener
{
public:
	using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;

	OscillatorSourceComponent(AudioProcessorValueTreeState& valueTreeState, String oscillatorName, const eden_vst::WaveTablePathProvider& pathProvider);

	void resized() override;

	void buttonClicked(Button*) override;

private:
	TextButton _isRealTime;
	float* _isRealTimeValue;

	ComboBox _waveform;
	std::unique_ptr<ComboBoxAttachment> _waveformAttachment;

	ComboBox _generator;
	std::unique_ptr<ComboBoxAttachment> _generatorAttachment;
};
