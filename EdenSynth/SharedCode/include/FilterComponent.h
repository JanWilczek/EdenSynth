#pragma once
///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include <JuceHeader.h>
#include "FilterControlsComponent.h"
#include "FilterEnvelopeComponent.h"
#include "parameters/ParameterRefs.h"

class FilterComponent : public Component {
public:
  explicit FilterComponent(const eden::plugin::ParameterRefs& parameters);

  void paint(Graphics& g) override;
  void resized() override;

private:
  FilterControlsComponent _filterControls;
  FilterEnvelopeComponent _filterEnvelope;
};
