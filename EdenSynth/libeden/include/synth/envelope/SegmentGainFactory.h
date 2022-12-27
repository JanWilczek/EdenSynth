#pragma once
///
/// \author Jan Wilczek
/// \date 04.11.2018
///
#include <memory>

namespace eden {
enum class EnvelopeSegmentCurve;
}

namespace eden::synth::envelope {
class ISegmentGain;

class SegmentGainFactory {
 public:
  /// <summary>
  /// Creates appropriate segment gain object depending on the <paramref
  /// name="segmentCurve"/> value.
  /// </summary>
  /// <param name="segmentCurve"></param>
  /// <returns>pointer to a concrete ISegmentGain object - e.g. to a LinearGain
  /// object when <paramref name="segmentCurve"/> is linear</returns>
  static std::unique_ptr<ISegmentGain> createSegmentGain(
      EnvelopeSegmentCurve segmentCurve);
};
}  // namespace eden::synth::envelope
