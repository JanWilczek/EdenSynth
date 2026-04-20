#pragma once
///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include <JuceHeader.h>
#include "EnvelopeSegmentComponent.h"
#include "parameters/ParameterRefs.h"

class EnvelopeComponent : public Component {
public:
  using SliderAttachment = juce::SliderParameterAttachment;

  EnvelopeComponent(AudioProcessorValueTreeState& valueTreeState,
                    const eden::plugin::ParameterRefs& parameters);

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
