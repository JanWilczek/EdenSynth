#pragma once
///
/// \author Jan Wilczek
/// \date 07.11.2018
///
#include <JuceHeader.h>

class EnvelopeSegmentComponent : public Component {
public:
  using SliderAttachment = juce::SliderParameterAttachment;
  using ComboBoxAttachment = juce::ComboBoxParameterAttachment;

  EnvelopeSegmentComponent(AudioProcessorValueTreeState& valueTreeState,
                           String segmentLabel,
                           String timeParameterID,
                           String curveParameterID,
                           juce::AudioParameterFloat& timeParameter,
                           juce::AudioParameterFloat& curveParameter);

  void resized() override;

private:
  Label _segmentName;

  Slider _time;
  std::unique_ptr<SliderAttachment> _timeAttachment;

  ComboBox _curve;
  std::unique_ptr<ComboBoxAttachment> _curveAttachment;
};
