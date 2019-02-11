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

class OscillatorSourceComponent : public Component
{
public:
	using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
	
	OscillatorSourceComponent(AudioProcessorValueTreeState& valueTreeState, String oscillatorName, const eden_vst::WaveTablePathProvider& pathProvider);

	void resized() override;

private:

	ComboBox _waveform;
	std::unique_ptr<ComboBoxAttachment> _waveformAttachment;
};
