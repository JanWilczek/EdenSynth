#pragma once
///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include <JuceHeader.h>
#include "EdenAdapter.h"
#include "FilterComponent.h"
#include "WaveshaperComponent.h"
#include "parameters/ParameterRefs.h"

class ModifierComponent : public Component {
public:
  ModifierComponent(const eden::plugin::ParameterRefs& parameters,
                    const eden::plugin::EdenAdapter& adapter);

  void resized() override;

private:
  FilterComponent _filterComponent;
  WaveshaperComponent _waveshaperComponent;
};
