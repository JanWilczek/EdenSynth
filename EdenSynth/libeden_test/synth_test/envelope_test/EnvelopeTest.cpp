///
/// \author Jan Wilczek
/// \date 01.11.2018
///

#include "synth/envelope/ADBDR.h"
#include "synth/envelope/Envelope.h"
#include "TestUtils.h"
#include "eden/EnvelopeParameters.h"

namespace libeden_test {
TEST(EnvelopeTest, SetSampleRate) {
  std::unique_ptr<eden::synth::envelope::Envelope> envelope =
      std::make_unique<eden::synth::envelope::ADBDR>(
          48000.f, eden::ADBDRParameters(
                       10ms, eden::EnvelopeSegmentCurve::Exponential, 10ms,
                       eden::EnvelopeSegmentCurve::Exponential, 10000ms,
                       eden::EnvelopeSegmentCurve::Exponential, 8000ms,
                       eden::EnvelopeSegmentCurve::Exponential, 0.7f));
  constexpr auto channelLength = 480u;
  float audioChannel[channelLength] = {float(0)};

  envelope->keyOn();
  for (auto i = 0; i < 2; ++i) {
    TestUtils::fillChannel(audioChannel, float(1), 0, channelLength);
    envelope->applyInRange(audioChannel, 0, channelLength);
  }

  EXPECT_NEAR(audioChannel[channelLength - 1], float(0.7f), 1e-2f);

  envelope->setSampleRate(44100.0);
  envelope->keyOff();

  // Least common multiple of 480 and 44,100 is 352,800.
  // Therefore it should take 352,800 / 44,100 = 8 seconds with this sample rate
  // for the envelope to reach 0.
  for (auto i = 0u; i < 735; ++i) {
    TestUtils::fillChannel(audioChannel, float(1), 0, channelLength);
    envelope->applyInRange(audioChannel, 0, channelLength);

    // Due to near 0 value, the envelope may end sooner - therefore don't check
    // the last hundred of iterations.
    if (i < 635) {
      EXPECT_NE(audioChannel[0], float(0));
    }
  }

  EXPECT_NEAR(audioChannel[channelLength - 1], float(0), 1e-3);
}
}  // namespace libeden_test
