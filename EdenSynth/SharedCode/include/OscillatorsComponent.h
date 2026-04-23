#pragma once
///
/// \author Jan Wilczek
/// \date 14.11.2018
///
#include <JuceHeader.h>
#include "OscillatorComponent.h"
#include "parameters/ParameterRefs.h"

namespace eden::plugin {
class WaveTablePathProvider;
}

class OscillatorsComponent : public Component {
public:
  OscillatorsComponent(
      const eden::plugin::OscillatorParametersContainer& parameters,
      const eden::plugin::WaveTablePathProvider& pathProvider);

  void paint(Graphics& g) override;
  void resized() override;

private:
  OscillatorComponent _oscillator1;
  OscillatorComponent _oscillator2;
  OscillatorComponent _oscillator3;
};
