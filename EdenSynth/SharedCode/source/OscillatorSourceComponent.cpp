///
/// \author Jan Wilczek
/// \date 14.11.2018
///
#include "OscillatorSourceComponent.h"
#include "WaveTablePathProvider.h"

OscillatorSourceComponent::OscillatorSourceComponent(
    AudioProcessorValueTreeState& valueTreeState,
    String oscillatorName,
    const eden::plugin::OscillatorParameters& parameters,
    const eden::plugin::WaveTablePathProvider& pathProvider)
    : _waveform("Waveform"), _generator("Generator name") {
  const auto auxParameterName = "gen." + oscillatorName + ".";

  addAndMakeVisible(_isRealTime);
  _isRealTimeAttachment = std::make_unique<juce::ButtonParameterAttachment>(
      parameters.isRealTime, _isRealTime);

  auto it = pathProvider.cbegin();
  auto i = 1;
  while (it != pathProvider.cend()) {
    _waveform.addItem(it->first, i);
    ++i;
    ++it;
  }

  addAndMakeVisible(_waveform);
  _waveformAttachment = std::make_unique<juce::ComboBoxParameterAttachment>(
      parameters.waveTable, _waveform);

  _generator.addItem("Sawtooth Ramp Up", 1);
  _generator.addItem("Square", 2);
  _generator.addItem("Triangle", 3);
  _generator.addItem("Sine", 4);
  _generator.addItem("White Noise", 5);

  addAndMakeVisible(_generator);
  _generatorAttachment = std::make_unique<juce::ComboBoxParameterAttachment>(
      parameters.generatorName, _generator);

  _isRealTime.addListener(this);

  buttonClicked(&_isRealTime);
}

void OscillatorSourceComponent::resized() {
  const auto labelHeight = 30;

  _isRealTime.setBounds(0, 5, getWidth(), labelHeight);
  _waveform.setBounds(0, getHeight() / 2, getWidth(), labelHeight);
  _generator.setBounds(_waveform.getBounds());
}

void OscillatorSourceComponent::buttonClicked(Button*) {
  if (_isRealTime.getToggleState()) {
    _isRealTime.setButtonText("Real time");
    _waveform.setVisible(false);
    _generator.setVisible(true);
  } else {
    _isRealTime.setButtonText("Wavetable");
    _generator.setVisible(false);
    _waveform.setVisible(true);
  }
}
