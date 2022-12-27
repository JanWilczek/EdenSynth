///
/// \author Jan Wilczek
/// \date 12.10.18
///
#include "synth/envelope/ADBDR.h"
#include "eden/EnvelopeParameters.h"
#include "synth/envelope/SegmentGainFactory.h"

namespace eden::synth::envelope {
ADBDR::ADBDR(float sampleRate, ADBDRParameters parameters)
    //: _breakLevel(parameters.breakLevel)
    : _attack(sampleRate,
              SegmentGainFactory::createSegmentGain(parameters.attackCurve),
              parameters.attackTime,
              0.0,
              1.0),
      _decay1(sampleRate,
              SegmentGainFactory::createSegmentGain(parameters.decay1Curve),
              parameters.decay1Time,
              1.0,
              parameters.breakLevel),
      _decay2(sampleRate,
              SegmentGainFactory::createSegmentGain(parameters.decay2Curve),
              parameters.decay2Time,
              parameters.breakLevel,
              0.0),
      _release(sampleRate,
               SegmentGainFactory::createSegmentGain(parameters.releaseCurve),
               parameters.releaseTime,
               parameters.breakLevel,
               0.0) {
  _segments.insert(_segments.begin() + static_cast<int>(ADBDRSegments::Attack),
                   &_attack);
  _segments.insert(_segments.begin() + static_cast<int>(ADBDRSegments::Decay1),
                   &_decay1);
  _segments.insert(_segments.begin() + static_cast<int>(ADBDRSegments::Decay2),
                   &_decay2);
  _segments.insert(_segments.begin() + static_cast<int>(ADBDRSegments::Release),
                   &_release);
  _segments.insert(_segments.begin() + static_cast<int>(ADBDRSegments::Silence),
                   &_silence);
}

void ADBDR::keyOn() {
  switchToSegment(static_cast<int>(ADBDRSegments::Attack));
}

void ADBDR::keyOff() {
  switchToSegment(static_cast<int>(ADBDRSegments::Release));
}

void ADBDR::setSegmentTime(ADBDRSegments segment,
                           std::chrono::milliseconds time) {
  _segments[static_cast<int>(segment)]->setDuration(time);
}

void ADBDR::setSegmentCurve(ADBDRSegments segment, EnvelopeSegmentCurve curve) {
  _segments[static_cast<int>(segment)]->setGainCurve(
      SegmentGainFactory::createSegmentGain(curve));
}

void ADBDR::setBreakLevel(float breakLevel) {
  _decay1.setFinalLevel(breakLevel);
  _decay2.setInitialLevel(breakLevel);
  _release.setInitialLevel(breakLevel);
}

bool ADBDR::hasEnded() {
  return _currentSegment >= static_cast<int>(ADBDRSegments::Silence);
}
}  // namespace eden::synth::envelope
