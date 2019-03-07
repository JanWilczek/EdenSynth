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
	//_isRealTimeValue = valueTreeState.getRawParameterValue(auxParameterName + "isRealTime");
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
	_generator.addItem("Square", 2);
	_generator.addItem("Triangle", 3);
	_generator.addItem("Sine", 4);

	addAndMakeVisible(_generator);
	_generatorAttachment = std::make_unique<ComboBoxAttachment>(valueTreeState, auxParameterName + "generatorName", _generator);

	_isRealTime.addListener(this);

	buttonClicked(&_isRealTime);
}

void OscillatorSourceComponent::resized()
{
	const auto labelHeight = 30;

	_isRealTime.setBounds(0, 5, getWidth(), labelHeight);
	_waveform.setBounds(0, getHeight() / 2, getWidth(), labelHeight);
	_generator.setBounds(_waveform.getBounds());
}

void OscillatorSourceComponent::buttonClicked(Button*)
{
	//*_isRealTimeValue = static_cast<float>(!static_cast<bool>(*_isRealTimeValue));

	//if (static_cast<bool>(*_isRealTimeValue))
	//{
	//	_isRealTime.setButtonText("Real time");
	//	_waveform.setVisible(false);
	//	_generator.setVisible(true);
	//}
	//else
	//{
	//	_isRealTime.setButtonText("Wavetable");
	//	_generator.setVisible(false);
	//	_waveform.setVisible(true);
	//}
}
