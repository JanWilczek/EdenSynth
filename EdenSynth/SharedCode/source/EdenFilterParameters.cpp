///
/// \author Jan Wilczek
/// \date 01.12.2018
///
#include "EdenFilterParameters.h"
#include <eden/EdenSynthesiser.h>

namespace eden::plugin {
EdenFilterParameters::EdenFilterParameters(eden::EdenSynthesiser& synthesiser)
    : _synthesiser(synthesiser) {}

void EdenFilterParameters::updateFilterParameters() {
  _synthesiser.setCutoff(*_cutoff);
  _synthesiser.setResonance(*_resonance);
  _synthesiser.setContourAmount(*_contourAmount);
  _synthesiser.setPassbandAttenuation(static_cast<eden::PassbandAttenuation>(
      static_cast<int>(*_passbandAttenuation)));

  std::shared_ptr<eden::ADSRParameters> filterEnvelopeParameters =
      std::make_shared<eden::ADSRParameters>();
  filterEnvelopeParameters->attackTime =
      std::chrono::milliseconds(static_cast<int>(*_attackTime));
  filterEnvelopeParameters->decayTime =
      std::chrono::milliseconds(static_cast<int>(*_decayTime));
  filterEnvelopeParameters->sustainLevel = *_sustainLevel;
  filterEnvelopeParameters->releaseTime =
      std::chrono::milliseconds(static_cast<int>(*_releaseTime));

  _synthesiser.setFilterEnvelopeParameters(filterEnvelopeParameters);
}
}  // namespace eden::plugin
