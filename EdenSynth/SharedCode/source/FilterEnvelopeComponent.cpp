///
/// \author Jan Wilczek
/// \date 02.12.2018
///
#include "FilterEnvelopeComponent.h"

FilterEnvelopeComponent::FilterEnvelopeComponent(
    const eden::plugin::ParameterRefs& parameters)
    : _attack{parameters.filterAttackTime, "Attack"},
      _decay{parameters.filterDecayTime, "Decay"},
      _sustain{parameters.filterSustainLevel, "Sustain"},
      _release{parameters.filterReleaseTime, "Release"} {
  addAndMakeVisible(_attack);
  addAndMakeVisible(_decay);
  addAndMakeVisible(_sustain);
  addAndMakeVisible(_release);
}

void FilterEnvelopeComponent::resized() {
  const int segmentWidth = getWidth() / 4;

  _attack.setBounds(0, 0, segmentWidth, getHeight());
  _decay.setBounds(_attack.getX() + segmentWidth, 0, segmentWidth, getHeight());
  _sustain.setBounds(_decay.getX() + segmentWidth, 0, segmentWidth,
                     getHeight());
  _release.setBounds(_sustain.getX() + segmentWidth, 0, segmentWidth,
                     getHeight());
}
