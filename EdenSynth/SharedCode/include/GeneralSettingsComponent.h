#pragma once
///
/// \author Jan Wilczek
/// \date 21.11.2018
///
#include "../JuceLibraryCode/JuceHeader.h"

class GeneralSettingsComponent : public Component {
 public:
  using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

  GeneralSettingsComponent(AudioProcessorValueTreeState&);

  void resized() override;
  void paint(Graphics& g) override;

 private:
  Label _pitchBendSemitonesUpLabel{"pitchBendSemitonesUpLabel",
                                   "Pitch bend up"};
  Slider _pitchBendSemitonesUp;
  std::unique_ptr<SliderAttachment> _pitchBendSemitonesUpAttachment;

  Label _pitchBendSemitonesDownLabel{"pitchBendSemitonesDownLabel",
                                     "Pitch bend down"};
  Slider _pitchBendSemitonesDown;
  std::unique_ptr<SliderAttachment> _pitchBendSemitonesDownAttachment;

  Label _a4FrequencyLabel{"a4FrequencyLabel", "A4 frequency"};
  Slider _a4Frequency;
  std::unique_ptr<SliderAttachment> _a4FrequencyAttachment;
};
