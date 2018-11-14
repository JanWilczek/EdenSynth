/// 
/// \author Jan Wilczek
/// \date 14.11.2018
/// 
#include "OscillatorComponent.h"
#include "WaveTablePathProvider.h"

OscillatorComponent::OscillatorComponent(AudioProcessorValueTreeState& valueTreeState, String oscillatorName, const eden_vst::WaveTablePathProvider& pathProvider)
	: _octaveTransposition(Slider::SliderStyle::Rotary, Slider::TextEntryBoxPosition::NoTextBox)
	, _semitoneTransposition(Slider::SliderStyle::Rotary, Slider::TextEntryBoxPosition::NoTextBox)
	, _centTransposition(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox)
	, _waveform("Waveform")
	, _on("ON")
	, _auxParameterName("generator." + oscillatorName + ".")
{
	_octaveLabel.setJustificationType(Justification::horizontallyCentred);
	addAndMakeVisible(_octaveLabel);
	_octaveTransposition.setPopupDisplayEnabled(true, false, this);
	addAndMakeVisible(_octaveTransposition);
	_octaveTranspositionAttachment = std::make_unique<SliderAttachment>(valueTreeState, _auxParameterName + "octaveTransposition", _octaveTransposition);

	_transposeLabel.setJustificationType(Justification::horizontallyCentred);
	addAndMakeVisible(_transposeLabel);
	_semitoneTransposition.setPopupDisplayEnabled(true, false, this);
	addAndMakeVisible(_semitoneTransposition);
	_semitoneTranspositionAttachment = std::make_unique<SliderAttachment>(valueTreeState, _auxParameterName + "semitoneTransposition", _semitoneTransposition);

	if (oscillatorName != "oscillator1")
	{
		_fineTuneLabel.setJustificationType(Justification::horizontallyCentred);
		addAndMakeVisible(_fineTuneLabel);
		_centTransposition.setPopupDisplayEnabled(true, false, this);
		addAndMakeVisible(_centTransposition);
		_centTranspositionAttachment = std::make_unique<SliderAttachment>(valueTreeState, _auxParameterName + "centTransposition", _centTransposition);
	}

	auto it = pathProvider.cbegin();
	auto i = 1;
	while (it != pathProvider.cend())
	{
		_waveform.addItem(it->first, i);
		++i;
		++it;
	}
	addAndMakeVisible(_waveform);
	_waveformAttachment = std::make_unique<ComboBoxAttachment>(valueTreeState, _auxParameterName + "waveTable", _waveform);

	addAndMakeVisible(_on);
	_onAttachment = std::make_unique<ButtonAttachment>(valueTreeState, _auxParameterName + "on", _on);
}

void OscillatorComponent::resized()
{
	const auto segmentWidth = getWidth() / 6;
	const auto labelHeight = 20;
	const auto knobDim = getHeight() - labelHeight - 10;
	
	_octaveLabel.setBounds(0, 0, segmentWidth, labelHeight);
	_octaveTransposition.setBounds((segmentWidth - knobDim) / 2, labelHeight, knobDim, knobDim);

	_transposeLabel.setBounds(_octaveLabel.getX() + segmentWidth, _octaveLabel.getY(), segmentWidth, labelHeight);
	_semitoneTransposition.setBounds(_octaveTransposition.getX() + segmentWidth, _octaveTransposition.getY(), knobDim, knobDim);

	_fineTuneLabel.setBounds(_transposeLabel.getX() + segmentWidth, _octaveLabel.getY(), segmentWidth, labelHeight);
	_centTransposition.setBounds(_semitoneTransposition.getX() + segmentWidth, _octaveTransposition.getY(), knobDim, knobDim);

	_waveform.setBounds(_fineTuneLabel.getX() + segmentWidth, getHeight() / 2, 1.5f * segmentWidth, labelHeight);

	_on.setBounds(_waveform.getX() + _waveform.getWidth(), _waveform.getY(), 0.5f * segmentWidth, _waveform.getHeight());
}
