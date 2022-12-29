#pragma once
///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include <memory>
#include <vector>
#include "eden/EnvelopeParameters.h"

namespace eden::synth::envelope {
class IEnvelopeHolder;
}

namespace eden::settings {
/// <summary>
/// Class responsible for changing settings of note envelopes belonging to all
/// voices. All registered envelopes will have the same envelope parameters and
/// thus have to be of the same type.
/// </summary>
class EnvelopeSettings {
public:
  EnvelopeSettings(float sampleRate);

  /// <summary>
  /// Registers an envelope for settings' control.
  /// </summary>
  /// <param name="envelope"></param>
  void registerEnvelope(
      std::shared_ptr<synth::envelope::IEnvelopeHolder> envelope);

  /// <summary>
  /// Sets sample rate of all registered envelopes.
  /// </summary>
  /// <param name="sampleRate"></param>
  void setSampleRate(float sampleRate);

  /// <summary>
  /// Sets envelope parameters of all registered envelopes.
  /// </summary>
  /// <param name="envelopeParameters"></param>
  void setEnvelopeParameters(
      std::shared_ptr<EnvelopeParameters> envelopeParameters);

private:
  /// <summary>
  /// Sets envelope settings with knowledge that they are parameters of an ADBDR
  /// envelope.
  /// </summary>
  /// <param name="adbdrParameters"></param>
  void setADBDRParameters(std::shared_ptr<ADBDRParameters> adbdrParameters);

  /// <summary>
  /// Sets envelope settings with knowledge that they are parameters of an ADSR
  /// envelope.
  /// </summary>
  /// <param name="adsrParameters"></param>
  void setADSRParameters(std::shared_ptr<ADSRParameters> adsrParameters);

  /// <summary>
  /// All registered envelopes.
  /// </summary>
  std::vector<std::shared_ptr<synth::envelope::IEnvelopeHolder>>
      _envelopeGenerators;

  /// <summary>
  /// Currently set envelope parameters.
  /// </summary>
  std::shared_ptr<EnvelopeParameters> _currentParameters;

  float _sampleRate;
};
}  // namespace eden::settings
