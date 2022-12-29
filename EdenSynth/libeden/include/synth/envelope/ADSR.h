#pragma once
///
/// \author Jan Wilczek
/// \date 27.11.2018
///
#include "synth/envelope/Attack.h"
#include "synth/envelope/Decay.h"
#include "synth/envelope/Envelope.h"
#include "synth/envelope/Sustain.h"

namespace eden {
enum class EnvelopeSegmentCurve;
struct ADSRParameters;
}  // namespace eden

namespace eden::synth::envelope {
class ADSR : public Envelope {
public:
  enum class ADSRSegments {
    Attack = 0,
    Decay = 1,
    Sustain = 2,
    Release = 3,
    Silence = 4
  };

  ADSR(float sampleRate, ADSRParameters parameters);
  ~ADSR() override = default;

  void keyOn() override;
  void keyOff() override;

  void setSegmentTime(ADSRSegments segment, std::chrono::milliseconds time);
  void setSegmentCurve(ADSRSegments segment, EnvelopeSegmentCurve curve);
  void setSustainLevel(float sustainLevel);

protected:
  bool hasEnded() override;

private:
  Attack _attack;
  Decay _decay;
  Sustain _sustain;
  Decay _release;
  Sustain _silence;
};
}  // namespace eden::synth::envelope
