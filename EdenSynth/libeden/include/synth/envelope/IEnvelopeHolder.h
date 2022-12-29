#pragma once
///
/// \author Jan Wilczek
/// \date 01.12.2018
///
#include <memory>

namespace eden::synth::envelope {
class Envelope;

/// <summary>
/// Container of a shared envelope.
/// </summary>
class IEnvelopeHolder {
public:
  virtual ~IEnvelopeHolder() = 0;

  /// <param name="envelope">pointer to the envelope to be stored</param>
  virtual void setEnvelope(std::shared_ptr<Envelope> envelope) = 0;

  /// <returns>pointer to the stored envelope</returns>
  virtual std::shared_ptr<Envelope> getEnvelope() const noexcept = 0;
};
inline IEnvelopeHolder::~IEnvelopeHolder() {}
}  // namespace eden::synth::envelope
