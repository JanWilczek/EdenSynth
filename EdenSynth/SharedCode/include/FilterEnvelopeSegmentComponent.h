#pragma once
///
/// \author Jan Wilczek
/// \date 02.12.2018
///
#include "../JuceLibraryCode/JuceHeader.h"

class FilterEnvelopeSegmentComponent : public Component {
public:
  using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

  FilterEnvelopeSegmentComponent(AudioProcessorValueTreeState&,
                                 String labelContent,
                                 String parameterID);

  void resized() override;

private:
  Label _label;
  Slider _slider;
  std::unique_ptr<SliderAttachment> _attachment;
};
