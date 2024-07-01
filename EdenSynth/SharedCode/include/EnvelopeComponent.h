#pragma once
///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include <JuceHeader.h>
#include "EnvelopeSegmentComponent.h"

class EnvelopeComponent : public Component {
public:
  using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

  EnvelopeComponent(AudioProcessorValueTreeState& valueTreeState);

  void paint(Graphics& g) override;
  void resized() override;

private:
  EnvelopeSegmentComponent _attack;
  EnvelopeSegmentComponent _decay1;
  EnvelopeSegmentComponent _decay2;
  EnvelopeSegmentComponent _release;

  Label _breakLevelLabel{"breakLevelLabel", "Break level"};
  Slider _breakLevel;
  std::unique_ptr<SliderAttachment> _breakLevelAttachment;
};
