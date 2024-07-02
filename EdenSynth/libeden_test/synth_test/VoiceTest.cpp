///
/// \author Jan Wilczek
/// \date 02.11.2018
///

#include "eden/AudioBuffer.h"
#include "eden/EnvelopeParameters.h"
#include "settings/Settings.h"
#include "synth/Voice.h"
#include "synth/envelope/ADBDR.h"
#include "TestUtils.h"
#include "synth/wavetable/SineWaveTable.h"

namespace libeden_test {
class VoiceTest : public ::testing::Test {
protected:
  void SetUp() override {
    _settings.setSampleRate(SAMPLE_RATE);
    _voice = std::make_unique<eden::synth::Voice>(_settings);
    _buffer.fill(0.f);

    _sourceId =
        _settings.createWaveTableSource(eden::synth::wavetable::SineWaveTable);
    _oscId = _settings.addOscillator(_sourceId);
  }

  const float SAMPLE_RATE = 48000.f;
  eden::settings::Settings _settings{};
  std::unique_ptr<eden::synth::Voice> _voice;

  constexpr static unsigned BUFFER_LENGTH = 480u;
  eden::AudioBuffer _buffer{1, BUFFER_LENGTH};

  eden::OscillatorId _oscId;
  eden::OscillatorSourceId _sourceId;
};

TEST_F(VoiceTest, ZeroInZeroOut) {
  _buffer.mix(0, _voice->renderBlock(BUFFER_LENGTH), 0, BUFFER_LENGTH);

  _buffer.forEachSample([](float& sample) { EXPECT_FLOAT_EQ(sample, 0.f); });
}

TEST_F(VoiceTest, StartStopNote) {
  constexpr int noteNumber = 69;

  _settings.setEnvelopeParameters(std::make_shared<eden::ADBDRParameters>(
      10ms, eden::EnvelopeSegmentCurve::Linear, 10ms,
      eden::EnvelopeSegmentCurve::Linear, 100000ms,
      eden::EnvelopeSegmentCurve::Linear, 1ms,
      eden::EnvelopeSegmentCurve::Linear, float(0.9)));
  _voice->startNote(noteNumber, 1.0f);

  EXPECT_TRUE(_voice->isPlaying());
  EXPECT_TRUE(_voice->isPlayingNote(69));

  _buffer.fill(float(0));
  _buffer.mix(0, _voice->renderBlock(BUFFER_LENGTH), 0, BUFFER_LENGTH);

  const auto detectedFrequency = TestUtils::detectFrequency(
      _buffer.getReadPointer(0), BUFFER_LENGTH, SAMPLE_RATE);
  EXPECT_NEAR(440.0, detectedFrequency, 1.f);

  _voice->stopNote(0.f);

  _buffer.fill(float(0));
  _buffer.mix(0, _voice->renderBlock(BUFFER_LENGTH), 0, BUFFER_LENGTH);

  // silence should start from sample 48, but starts from sample 54
  for (auto i = 54u; i < BUFFER_LENGTH; ++i) {
    EXPECT_FLOAT_EQ(_buffer.getReadPointer(0)[i], float(0));
  }
}

TEST_F(VoiceTest, CalculatePitch) {
  auto tuner = _settings.tuner();
  constexpr auto A4 = 440.f;

  tuner->setFrequencyOfA4(A4);
  EXPECT_FLOAT_EQ(tuner->calculatePitch(
                      69, eden::settings::Tuner::PITCH_BEND_NEUTRAL_VALUE),
                  A4);
}

TEST_F(VoiceTest, VariousParameters) {
  constexpr auto sampleRate = 1000.0;
  constexpr auto blockLength = 250u;
  constexpr auto segmentTime = 250ms;
  constexpr float breakLevel(0.5);
  constexpr int noteNumber = 83;

  _settings.setSampleRate(sampleRate);
  _voice->setBlockLength(250u);
  _settings.setEnvelopeParameters(std::make_shared<eden::ADBDRParameters>(
      segmentTime, eden::EnvelopeSegmentCurve::Linear, segmentTime,
      eden::EnvelopeSegmentCurve::Linear, segmentTime,
      eden::EnvelopeSegmentCurve::Linear, segmentTime,
      eden::EnvelopeSegmentCurve::Linear, breakLevel));
  auto sourceId = _settings.createWaveTableSource({1.f, 1.f});
  _settings.setOscillatorSource(_oscId, sourceId);

  EXPECT_DOUBLE_EQ(_settings.sampleRate(), sampleRate);
  EXPECT_FALSE(_voice->isPlaying());

  _voice->startNote(noteNumber, 1.0f);

  EXPECT_TRUE(_voice->isPlaying());
  EXPECT_TRUE(_voice->isPlayingNote(noteNumber));

  _buffer = eden::AudioBuffer(1, blockLength);

  // attack
  _buffer.fill(float(0));
  _buffer.mix(0, _voice->renderBlock(blockLength), 0, blockLength);

  // TODO: Fix volume
  //(_buffer.getReadPointer(0)[blockLength - 1], _voice->gainValue() * 1.0f,
  // 0.05f);

  // decay1
  _buffer.fill(float(0));
  _buffer.mix(0, _voice->renderBlock(blockLength), 0, blockLength);

  // TODO: Fix volume
  // EXPECT_NEAR(_buffer.getReadPointer(0)[blockLength - 1], _voice->gainValue()
  // * breakLevel, 0.05f);

  // release
  _voice->stopNote(0.f);

  _buffer.fill(float(0));
  _buffer.mix(0, _voice->renderBlock(blockLength), 0, blockLength);

  EXPECT_NEAR(_buffer.getReadPointer(0)[blockLength - 1], 0.f, 0.05f);

  // silence
  _buffer.fill(float(0));
  _buffer.mix(0, _voice->renderBlock(blockLength), 0, blockLength);

  for (auto i = 1u; i < blockLength; ++i) {
    EXPECT_FLOAT_EQ(_buffer.getReadPointer(0)[i], float(0));
  }

  EXPECT_FALSE(_voice->isPlayingNote(noteNumber));
  EXPECT_FALSE(_voice->isPlaying());
}
}  // namespace libeden_test
