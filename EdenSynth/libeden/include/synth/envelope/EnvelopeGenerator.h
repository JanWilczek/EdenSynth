#pragma once
///
/// \author Jan Wilczek
/// \date 27.11.2018
///
#include "synth/envelope/Envelope.h"
#include "synth/envelope/IEnvelopeHolder.h"

namespace eden::synth::envelope {
/// <summary>
/// Class responsible for applying envelope to the given signal.
/// </summary>
class EnvelopeGenerator : public IEnvelopeHolder {
public:
  /// <param name="envelope">envelope to create the generator with</param>
  explicit EnvelopeGenerator(std::shared_ptr<Envelope> envelope);

  /// <summary>
  /// Applies envelope on samples in channel in range [<paramref
  /// name="startSample"/>, <paramref name="startSample"/> + <paramref
  /// name="samplesToApply"/>).
  /// </summary>
  /// <param name="channel"></param>
  /// <param name="startSample"></param>
  /// <param name="samplesToApply"></param>
  void apply(float* channel, int startSample, int samplesToApply);

  /// <summary>
  /// Signal the key on event.
  /// </summary>
  void keyOn();

  /// <summary>
  /// Signal the key off event
  /// </summary>
  void keyOff();

  void setEnvelope(std::shared_ptr<Envelope> envelope) override;
  std::shared_ptr<Envelope> getEnvelope() const noexcept override;

  /// <summary>
  /// Sets the function to be called when the envelope has ended.
  /// Sample index at which the envelope has ended is passed to it.
  /// </summary>
  /// <param name="callback"></param>
  void setOnEnvelopeEndedCallback(Envelope::OnEnvelopeEnded callback);

private:
  /// <summary>
  /// Currently set envelope.
  /// </summary>
  std::shared_ptr<Envelope> _envelope;

  /// <summary>
  /// Function to invoke when the envelope has ended.
  /// </summary>
  Envelope::OnEnvelopeEnded _callback;
};
}  // namespace eden::synth::envelope
