#pragma once
///
/// \author Jan Wilczek
/// \date 21.11.2018
///
#include <JuceHeader.h>
#include "EdenAdapter.h"

class GeneralSettingsComponent : public Component {
public:
  explicit GeneralSettingsComponent(const eden::plugin::ParameterRefs&);

  void resized() override;
  void paint(Graphics& g) override;

private:
  Label _pitchBendSemitonesUpLabel{"pitchBendSemitonesUpLabel",
                                   "Pitch bend up"};
  Slider _pitchBendSemitonesUp;
  std::unique_ptr<juce::SliderParameterAttachment>
      _pitchBendSemitonesUpAttachment;

  Label _pitchBendSemitonesDownLabel{"pitchBendSemitonesDownLabel",
                                     "Pitch bend down"};
  Slider _pitchBendSemitonesDown;
  std::unique_ptr<juce::SliderParameterAttachment>
      _pitchBendSemitonesDownAttachment;

  Label _a4FrequencyLabel{"a4FrequencyLabel", "A4 frequency"};
  Slider _a4Frequency;
  std::unique_ptr<juce::SliderParameterAttachment> _a4FrequencyAttachment;
};
