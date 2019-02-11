/// 
/// \author Jan Wilczek
/// \date 14.11.2018
/// 
#include "OscillatorSourceComponent.h"
#include "WaveTablePathProvider.h"

OscillatorSourceComponent::OscillatorSourceComponent(AudioProcessorValueTreeState& valueTreeState, String oscillatorName, const eden_vst::WaveTablePathProvider& pathProvider)
	: _waveform("Waveform")
	, _generator("Generator name")
{
	const auto auxParameterName = "generator." + oscillatorName + ".";

	addAndMakeVisible(_isRealTime);
	_isRealTimeAttachment = std::make_unique<ButtonAttachment>(valueTreeState, auxParameterName + "isRealTime", _isRealTime);

	auto it = pathProvider.cbegin();
	auto i = 1;
	while (it != pathProvider.cend())
	{
		_waveform.addItem(it->first, i);
		++i;
		++it;
	}

	addAndMakeVisible(_waveform);
	_waveformAttachment = std::make_unique<ComboBoxAttachment>(valueTreeState, auxParameterName + "waveTable", _waveform);

	_generator.addItem("Sawtooth Ramp Up", 1);
	addAndMakeVisible(_generator);
	_generatorAttachment = std::make_unique<ComboBoxAttachment>(valueTreeState, auxParameterName + "generatorName", _generator);
	_generator.setVisible(false);

	_isRealTime.addListener(this);
}

void OscillatorSourceComponent::resized()
{
	const auto labelHeight = 30;

	_isRealTime.setBounds(0, 0, getWidth(), labelHeight);
	_waveform.setBounds(0, getHeight() / 2, getWidth(), labelHeight);
	_generator.setBounds(_waveform.getBounds());
}

void OscillatorSourceComponent::buttonClicked(Button* button)
{
	if (button->getToggleState())
	{
		_waveform.setVisible(false);
		_generator.setVisible(true);
	}
	else
	{
		_waveform.setVisible(true);
		_generator.setVisible(false);
	}
}
