#pragma once
///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include <memory>
#include <vector>
#include "eden/EnvelopeParameters.h"
#include "eden/FilterParameters.h"
#include "settings/EnvelopeSettings.h"

namespace eden::synth::subtractive {
class SubtractiveModule;
}

namespace eden::settings {
/// <summary>
/// Class responsible for updating parameters of all registered subtractive
/// modules.
/// </summary>
class SubtractiveModuleSettings {
public:
  explicit SubtractiveModuleSettings(float sampleRate);

  /// <summary>
  /// Registers given subtractive module for parameter update. From now on all
  /// of its settings will be updated automatically.
  /// </summary>
  /// <param name="subtractiveModule"></param>
  void registerSubtractiveModule(
      std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule);

  void setSampleRate(float sampleRate);

  /// <summary>
  /// Sets the cutoff frequency of the low-pass filter.
  /// </summary>
  /// <param name="cutoff">number of harmonic to set the cutoff frequency at (1
  /// is the fundamental), may be non-integer</param>
  void setCutoff(float cutoff);

  /// <summary>
  /// Set resonance or Q of the low-pass filter.Resonance boosts gain at cutoff
  /// frequency of the filter changing into ringing when the value is near 1.
  /// </summary>
  /// <param name="resonance">boost at cutoff frequency in range [0, 1]</param>
  void setResonance(float resonance);

  /// <summary>
  /// Sets the range in which filter's envelope affects its cutoff frequency.
  /// E.g. 1 means that the cutoff frequency will sweep through all frequencies
  /// from 0 to cutoff frequency during attack, while 0 means that filter's
  /// cutoff frequency will be steady (determined solely by the cutoff
  /// parameter, pitch and possibly modulation).
  /// </summary>
  /// <param name="contourAmount">scale in range [0, 1]</param>
  void setContourAmount(float contourAmount);

  /// <summary>
  /// Sets the attenuation of the filter in the pass-band. May be 12 dB per
  /// octave or 24 dB per octave.
  /// </summary>
  /// <param name="passbandAttenuation"></param>
  void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);

  /// <summary>
  /// Sets new envelope of the filter's cutoff frequency.
  /// E.g. long attack times create a feeling of the note 'opening up' as the
  /// cutoff frequency gradually increases.
  /// </summary>
  /// <param name="parameters">parameters of the filter's envelope to set -
  /// <c>ADSRParameters</c> struct instance for example</param>
  void setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> parameters);

private:
  /// <summary>
  /// Subtractive modules registered for settings' updates.
  /// </summary>
  std::vector<std::shared_ptr<synth::subtractive::SubtractiveModule>>
      _subtractiveModules;
  float _cutoff = 100.f;
  float _resonance = 0.f;
  float _contourAmount = 0.f;
  PassbandAttenuation _passbandAttenuation = PassbandAttenuation::dB12;

  /// <summary>
  /// Settings of registered modules' filters' envelopes.
  /// </summary>
  std::unique_ptr<EnvelopeSettings> _filterEnvelopeSettings;
};
}  // namespace eden::settings
