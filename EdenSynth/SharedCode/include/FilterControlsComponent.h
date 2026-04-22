#pragma once
///
/// \author Jan Wilczek
/// \date 02.12.2018
///
#include <JuceHeader.h>
#include "parameters/ParameterRefs.h"

class FilterControlsComponent : public Component {
public:
  using SliderAttachment = juce::SliderParameterAttachment;
  using ComboBoxAttachment = juce::ComboBoxParameterAttachment;

  explicit FilterControlsComponent(
      const eden::plugin::ParameterRefs& parameters);

  void resized() override;

private:
  Label _cutoffLabel{"cutoffLabel", "Cutoff"};
  Slider _cutoff;
  std::unique_ptr<SliderAttachment> _cutoffAttachment;

  Label _resonanceLabel{"resonanceLabel", "Resonance"};
  Slider _resonance;
  std::unique_ptr<SliderAttachment> _resonanceAttachment;

  Label _contourAmountLabel{"contourAmountLabel", "Contour\namount"};
  Slider _contourAmount;
  std::unique_ptr<SliderAttachment> _contourAmountAttachment;

  Label _passbandAttenuationLabel{"passbandAttenuationLabel",
                                  "Passband\nattenuation"};
  ComboBox _passbandAttenuation{"passbandAttenuation"};
  std::unique_ptr<ComboBoxAttachment> _passbandAttenuationAttachment;
};
