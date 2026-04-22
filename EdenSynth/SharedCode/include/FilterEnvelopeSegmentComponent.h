#pragma once
///
/// \author Jan Wilczek
/// \date 02.12.2018
///
#include <JuceHeader.h>

class FilterEnvelopeSegmentComponent : public Component {
public:
  using SliderAttachment = juce::SliderParameterAttachment;

  FilterEnvelopeSegmentComponent(juce::AudioParameterFloat& parameterToControl,
                                 String labelContent);

  void resized() override;

private:
  Label _label;
  Slider _slider;
  std::unique_ptr<SliderAttachment> _attachment;
};
