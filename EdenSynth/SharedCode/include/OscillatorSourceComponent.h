#pragma once
///
/// \author Jan Wilczek
/// \date 11.02.2019
///
#include <JuceHeader.h>
#include "parameters/ParameterRefs.h"

namespace eden::plugin {
class WaveTablePathProvider;
}

class OscillatorSourceComponent : public Component, public Button::Listener {
public:
  OscillatorSourceComponent(
      String oscillatorName,
      const eden::plugin::OscillatorParameters& parameters,
      const eden::plugin::WaveTablePathProvider& pathProvider);

  void resized() override;

  void buttonClicked(Button*) override;

private:
  ToggleButton _isRealTime;
  std::unique_ptr<juce::ButtonParameterAttachment> _isRealTimeAttachment;

  ComboBox _waveform;
  std::unique_ptr<juce::ComboBoxParameterAttachment> _waveformAttachment;

  ComboBox _generator;
  std::unique_ptr<juce::ComboBoxParameterAttachment> _generatorAttachment;
};
