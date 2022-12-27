///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include "GeneratorComponent.h"
#include "WaveTablePathProvider.h"

GeneratorComponent::GeneratorComponent(
    AudioProcessorValueTreeState& valueTreeState,
    const eden_vst::WaveTablePathProvider& pathProvider)
    : _oscillators(valueTreeState, pathProvider),
      _envelopeComponent(valueTreeState) {
  addAndMakeVisible(_oscillators);
  addAndMakeVisible(_envelopeComponent);
}

void GeneratorComponent::resized() {
  const auto oscillatorsComponentHeight = 300;

  _oscillators.setBounds(0, 0, getWidth(), oscillatorsComponentHeight);
  _envelopeComponent.setBounds(0, oscillatorsComponentHeight, getWidth(),
                               getHeight() - oscillatorsComponentHeight);
}
