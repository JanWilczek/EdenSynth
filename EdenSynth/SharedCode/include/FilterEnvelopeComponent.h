#pragma once
///
/// \author Jan Wilczek
/// \date 02.12.2018
///
#include <JuceHeader.h>
#include "FilterEnvelopeSegmentComponent.h"
#include "parameters/ParameterRefs.h"

class FilterEnvelopeComponent : public Component {
public:
  explicit FilterEnvelopeComponent(
      const eden::plugin::ParameterRefs& parameters);

  void resized() override;

private:
  FilterEnvelopeSegmentComponent _attack;
  FilterEnvelopeSegmentComponent _decay;
  FilterEnvelopeSegmentComponent _sustain;
  FilterEnvelopeSegmentComponent _release;
};
