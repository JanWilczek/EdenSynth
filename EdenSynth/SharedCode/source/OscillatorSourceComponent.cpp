/// 
/// \author Jan Wilczek
/// \date 14.11.2018
/// 
#include "OscillatorSourceComponent.h"
#include "WaveTablePathProvider.h"

OscillatorSourceComponent::OscillatorSourceComponent(AudioProcessorValueTreeState& valueTreeState, String oscillatorName, const eden_vst::WaveTablePathProvider& pathProvider)
	: _waveform("Waveform")
{
	auto it = pathProvider.cbegin();
	auto i = 1;
	while (it != pathProvider.cend())
	{
		_waveform.addItem(it->first, i);
		++i;
		++it;
	}

	addAndMakeVisible(_waveform);
	const auto auxParameterName = "generator." + oscillatorName + ".";
	_waveformAttachment = std::make_unique<ComboBoxAttachment>(valueTreeState, auxParameterName + "waveTable", _waveform);
}

void OscillatorSourceComponent::resized()
{
	const auto labelHeight = 30;

	_waveform.setBounds(0, getHeight() / 2, getWidth(), labelHeight);
}

