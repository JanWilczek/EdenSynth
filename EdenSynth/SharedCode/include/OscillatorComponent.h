#pragma once
/// 
/// \author Jan Wilczek
/// \date 14.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

namespace eden_vst
{
	class WaveTablePathProvider;
}

class OscillatorComponent : public Component
{
public:
	using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
	using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
	using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;

	OscillatorComponent(AudioProcessorValueTreeState& valueTreeState, String oscillatorName, const eden_vst::WaveTablePathProvider& pathProvider);

	void resized() override;

private:
	Label _octaveLabel{ "octaveTranspositionLabel", "Octave" };
	Slider _octaveTransposition;
	std::unique_ptr<SliderAttachment> _octaveTranspositionAttachment;

	Label _transposeLabel{ "semitoneTranspositionLabel", "Transpose" };
	Slider _semitoneTransposition;
	std::unique_ptr<SliderAttachment> _semitoneTranspositionAttachment;

	Label _fineTuneLabel{ "fineTuneLabel", "Fine\ntune" };
	Slider _centTransposition;
	std::unique_ptr<SliderAttachment> _centTranspositionAttachment;

	ComboBox _waveform;
	std::unique_ptr<ComboBoxAttachment> _waveformAttachment;

	ToggleButton _on;
	std::unique_ptr<ButtonAttachment> _onAttachment;

	Label _volumeLabel{ "oscillatorVolumeLabel", "Volume" };
	Slider _volume;
	std::unique_ptr<SliderAttachment> _volumeAttachment;

	String _auxParameterName;
};
