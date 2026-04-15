#pragma once
///
/// \author Jan Wilczek
/// \date 11.02.2019
///
#include <JuceHeader.h>

namespace eden::plugin {
class WaveTablePathProvider;
}

class OscillatorSourceComponent : public Component, public Button::Listener {
public:
  using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
  using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;

  OscillatorSourceComponent(
      AudioProcessorValueTreeState& valueTreeState,
      String oscillatorName,
      const eden::plugin::WaveTablePathProvider& pathProvider);

  void resized() override;

  void buttonClicked(Button*) override;

private:
  ToggleButton _isRealTime;
  std::unique_ptr<ButtonAttachment> _isRealTimeAttachment;

  ComboBox _waveform;
  std::unique_ptr<ComboBoxAttachment> _waveformAttachment;

  ComboBox _generator;
  std::unique_ptr<ComboBoxAttachment> _generatorAttachment;
};
