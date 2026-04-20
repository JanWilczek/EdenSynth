#pragma once
///
/// \author Jan Wilczek
/// \date 21.11.2018
///
#include <JuceHeader.h>
#include "parameters/ParameterRefs.h"

class OutputSettingsComponent : public Component {
public:
  using SliderAttachment = juce::SliderParameterAttachment;

  explicit OutputSettingsComponent(const eden::plugin::ParameterRefs&);

  void resized() override;
  void paint(Graphics& g) override;

private:
  Label _volumeLabel{"globalVolumeLabel", "Volume"};
  Slider _volume;
  std::unique_ptr<SliderAttachment> _volumeAttachment;
};
