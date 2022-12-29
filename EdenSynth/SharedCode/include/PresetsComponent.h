#pragma once

#include <JuceHeader.h>

class PresetsComponent : public juce::Component {
public:
  PresetsComponent();
  ~PresetsComponent() override;

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsComponent)
};
