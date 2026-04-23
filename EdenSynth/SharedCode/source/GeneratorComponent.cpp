///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include "GeneratorComponent.h"
#include "WaveTablePathProvider.h"

GeneratorComponent::GeneratorComponent(
    const eden::plugin::ParameterRefs& parameters,
    const eden::plugin::WaveTablePathProvider& pathProvider)
    : _oscillators(parameters.oscillators, pathProvider),
      _envelopeComponent(parameters) {
  addAndMakeVisible(_oscillators);
  addAndMakeVisible(_envelopeComponent);
}

void GeneratorComponent::resized() {
  const auto oscillatorsComponentHeight = 300;

  _oscillators.setBounds(0, 0, getWidth(), oscillatorsComponentHeight);
  _envelopeComponent.setBounds(0, oscillatorsComponentHeight, getWidth(),
                               getHeight() - oscillatorsComponentHeight);
}
