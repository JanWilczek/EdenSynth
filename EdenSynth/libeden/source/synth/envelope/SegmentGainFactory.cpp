///
/// \author Jan Wilczek
/// \date 04.11.2018
///
#include "synth/envelope/SegmentGainFactory.h"
#include <stdexcept>
#include "eden/EnvelopeParameters.h"
#include "synth/envelope/ExponentialGain.h"
#include "synth/envelope/LinearGain.h"

namespace eden::synth::envelope {
std::unique_ptr<ISegmentGain> SegmentGainFactory::createSegmentGain(
    EnvelopeSegmentCurve segmentCurve) {
  switch (segmentCurve) {
    case EnvelopeSegmentCurve::Linear:
      return std::make_unique<LinearGain>();
    case EnvelopeSegmentCurve::Exponential:
      return std::make_unique<ExponentialGain>();
    default:
      throw std::logic_error("Invalid EnvelopeSegmentCurve enum class value.");
  }
}
}  // namespace eden::synth::envelope
