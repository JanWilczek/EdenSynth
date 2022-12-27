#pragma once
///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeComponent.h"
#include "OscillatorsComponent.h"

class GeneratorComponent : public Component {
 public:
  GeneratorComponent(AudioProcessorValueTreeState& valueTreeState,
                     const eden_vst::WaveTablePathProvider& pathProvider);

  void resized() override;

 private:
  OscillatorsComponent _oscillators;
  EnvelopeComponent _envelopeComponent;
};
