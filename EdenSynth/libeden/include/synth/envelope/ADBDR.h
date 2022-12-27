#pragma once
///
/// \author Jan Wilczek
/// \date 11.10.2018
///
#include "synth/envelope/Attack.h"
#include "synth/envelope/Decay.h"
#include "synth/envelope/Envelope.h"
#include "synth/envelope/Sustain.h"

namespace eden {
enum class EnvelopeSegmentCurve;
struct ADBDRParameters;
}  // namespace eden

namespace eden::synth::envelope {
/// <summary>
/// Attack Decay1 Break Decay2 Release envelope. Similar to ADSR envelope, but
/// contains Decay2 instead of Sustain and therefore is better for simulating
/// piano-like envelope - one which fades slowly with the key pressed.
/// </summary>
class ADBDR : public Envelope {
 public:
  enum class ADBDRSegments {
    Attack = 0,
    Decay1 = 1,
    Decay2 = 2,
    Release = 3,
    Silence = 4
  };

  ADBDR(float sampleRate, ADBDRParameters parameters);
  ~ADBDR() override = default;

  void keyOn() override;
  void keyOff() override;

  void setSegmentTime(ADBDRSegments segment, std::chrono::milliseconds time);
  void setSegmentCurve(ADBDRSegments segment, EnvelopeSegmentCurve curve);
  void setBreakLevel(float breakLevel);

 protected:
  bool hasEnded() override;

 private:
  /// <summary>
  /// Value of envelope at which segments change from Decay 1 to Decay 2.
  /// </summary>
  // float _breakLevel;

  /// <summary>
  /// ADBDR envelope's segments.
  /// </summary>
  Attack _attack;
  Decay _decay1;
  Decay _decay2;
  Decay _release;
  Sustain _silence;
};
}  // namespace eden::synth::envelope
