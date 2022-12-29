#pragma once
///
///	\author Jan Wilczek
///	 \date 08.10.2018
///
#include "synth/IMonoModule.h"
#include "synth/envelope/Envelope.h"
#include "synth/envelope/IEnvelopeHolder.h"
#include "synth/subtractive/MoogFilter.h"

namespace eden {
enum class PassbandAttenuation;
}

namespace eden::synth::subtractive {
/// <summary>
/// Module responsible for filtering the generated signal.
/// </summary>
class SubtractiveModule : public IMonoModule, public envelope::IEnvelopeHolder {
public:
  explicit SubtractiveModule(float sampleRate);
  ~SubtractiveModule() override = default;

  /// <summary>
  /// Apply filtering on <paramref name="audioChannel"/> in range [<paramref
  /// name="startSample"/>, <paramref name="startSample"/> + <paramref
  /// name="samplesToProcess"/>).
  /// </summary>
  /// <param name="audioChannel"></param>
  /// <param name="startSample"></param>
  /// <param name="samplesToProcess"></param>
  void process(float* audioChannel,
               int startSample,
               int samplesToProcess) override;

  /// <summary>
  /// Signal the key on event.
  /// </summary>
  void keyOn();

  /// <summary>
  /// Signal the key off event.
  /// </summary>
  void keyOff();

  /// <summary>
  /// Sets cutoff, i.e. number of partial to set the cutoff frequency at. 1
  /// means that the cutoff frequency will be set at the fundamental frequency.
  /// </summary>
  /// <param name="cutoff">number of partial to set the cutoff frequency
  /// at</param>
  void setCutoff(float cutoff);

  /// <summary>
  /// Sets the size of the resonance (peak) at the cutoff frequency.
  /// </summary>
  /// <param name="resonance">cutoff frequency boost in range [0, 1]</param>
  void setResonance(float resonance);

  /// <summary>
  /// Sets the extent to which cutoff frequency is affected by filter's
  /// envelope: <see cref="_contourAmount"/>.
  /// </summary>
  /// <param name="contourAmount">envelope scale in range [0, 1]</param>
  void setContourAmount(float contourAmount);

  /// <summary>
  /// Sets the attenuation strength in the passband of the filter. 12 dB per
  /// octave means gentle attenuation, 24 dB per octave means strong (steep)
  /// attenuation.
  /// </summary>
  /// <param name="passbandAttenuation"></param>
  void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);

  /// <summary>
  /// Sets envelope of the filter's cutoff frequency.
  /// </summary>
  /// <param name="envelope"></param>
  void setEnvelope(std::shared_ptr<envelope::Envelope> envelope) override;

  /// <returns>current filter envelope</returns>
  std::shared_ptr<envelope::Envelope> getEnvelope() const noexcept override;

  void setSampleRate(float sampleRate);

  /// <summary>
  /// Sets pitch of the currently played note. Use it to have pitch- or
  /// key-follow functionality.
  /// </summary>
  /// <param name="pitch">frequency in Hz</param>
  void setPitch(float pitch);

private:
  /// <summary>
  /// Sets the cutoff frequency in Hz of inner filter based on <see
  /// cref="_cutoff"/>, <see cref="_contourAmount"/>, <see cref="_pitch"/> and
  /// <see cref="_cutoffFrequencyEnvelope"/>.
  /// </summary>
  void setCutoffFrequency();

  /// <summary>
  /// Virtual analog emulation of Moog ladder filter.
  /// </summary>
  MoogFilter _filter;

  /// <summary>
  /// Envelope of the filter's cutoff frequency. It's peak is always at
  /// <c>_cutoff</c> * <c>_pitch</c>.
  /// </summary>
  std::shared_ptr<envelope::Envelope> _cutoffFrequencyEnvelope;

  /// <summary>
  /// Number of harmonic to set the cutoff frequency at.
  /// </summary>
  float _cutoff;

  /// <summary>
  /// Determines the factor by which transform the filter envelope.
  /// If envelope's minimum is 0 and maximum is 1 then after transformation the
  /// minimum will be (1 - <c>_contourAmount</c>) and maximum 1. The factor is
  /// thus responsible for amplitude scale of the envelope.
  /// </summary>
  float _contourAmount;

  /// <summary>
  /// Current pitch of the voice the module belongs to.
  /// </summary>
  float _pitch;
};
}  // namespace eden::synth::subtractive
