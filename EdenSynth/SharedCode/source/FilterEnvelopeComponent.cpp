///
/// \author Jan Wilczek
/// \date 02.12.2018
///
#include "FilterEnvelopeComponent.h"

FilterEnvelopeComponent::FilterEnvelopeComponent(
    AudioProcessorValueTreeState& valueTreeState)
    : _attack(valueTreeState, "Attack", "filter.envelope.adsr.attack.time"),
      _decay(valueTreeState, "Decay", "filter.envelope.adsr.decay.time"),
      _sustain(valueTreeState, "Sustain", "filter.envelope.adsr.sustain.level"),
      _release(valueTreeState, "Release", "filter.envelope.adsr.release.time") {
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
