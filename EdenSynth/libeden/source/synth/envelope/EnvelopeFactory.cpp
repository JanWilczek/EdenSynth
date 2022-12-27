///
/// \author Jan Wilczek
/// \date 04.11.2018
///
#include "synth/envelope/EnvelopeFactory.h"
#include <stdexcept>
#include "eden/EnvelopeParameters.h"
#include "synth/envelope/ADBDR.h"
#include "synth/envelope/ADSR.h"

namespace eden::synth::envelope {
std::unique_ptr<Envelope> EnvelopeFactory::createEnvelope(
    float sampleRate,
    std::shared_ptr<EnvelopeParameters> parameters) {
  if (const auto adbdrParameters =
          std::dynamic_pointer_cast<ADBDRParameters>(parameters);
      adbdrParameters != nullptr) {
    return createADBDREnvelope(sampleRate, adbdrParameters);
  }
  if (const auto adsrParameters =
          std::dynamic_pointer_cast<ADSRParameters>(parameters);
      adsrParameters != nullptr) {
    return createADSREnvelope(sampleRate, adsrParameters);
  }

  throw std::runtime_error("Invalid envelope parameters.");
}

std::unique_ptr<Envelope> EnvelopeFactory::createADBDREnvelope(
    float sampleRate,
    std::shared_ptr<ADBDRParameters> parameters) {
  return std::make_unique<ADBDR>(sampleRate, *parameters);
}

std::unique_ptr<Envelope> EnvelopeFactory::createADSREnvelope(
    float sampleRate,
    std::shared_ptr<ADSRParameters> parameters) {
  return std::make_unique<ADSR>(sampleRate, *parameters);
}
}  // namespace eden::synth::envelope
