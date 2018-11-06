/// 
/// \author Jan Wilczek
/// \date 02.11.2018
/// 
#include "pch.h"
#include <synth/Voice.h>
#include <eden/AudioBuffer.h>
#include <synth/envelope/ADBDR.h>
#include <eden/EnvelopeParameters.h>
#include <settings/Settings.h>
#include "TestUtils.h"

namespace libeden_test
{
	class VoiceTest : public ::testing::Test
	{
	protected:
		void SetUp() override
		{
			_voice = std::make_unique<eden::synth::Voice>(_settings, SAMPLE_RATE);
			_buffer.fill(eden::SampleType(0));
		}

		const double SAMPLE_RATE = 48000.0;
		eden::settings::Settings _settings{};
		std::unique_ptr<eden::synth::Voice> _voice;

		constexpr static unsigned BUFFER_LENGTH = 480u;
		eden::AudioBuffer _buffer{ 1, BUFFER_LENGTH };
	};

	TEST_F(VoiceTest, ZeroInZeroOut)
	{
		_voice->renderBlock(_buffer, 0, BUFFER_LENGTH);

		_buffer.forEachSample([](eden::SampleType& sample)
		{
			EXPECT_FLOAT_EQ(sample, eden::SampleType(0));
		});
	}

	TEST_F(VoiceTest, StartStopNote)
	{
		constexpr int noteNumber = 69;

		_voice->setEnvelope(std::make_unique<eden::synth::envelope::ADBDR>(SAMPLE_RATE, eden::ADBDRParameters(10ms, eden::EnvelopeSegmentCurve::Linear,
			10ms, eden::EnvelopeSegmentCurve::Linear, 100000ms, eden::EnvelopeSegmentCurve::Linear, 1ms, eden::EnvelopeSegmentCurve::Linear, eden::SampleType(0.9))));
		_voice->startNote(noteNumber, 1.0f);

		EXPECT_TRUE(_voice->isPlaying());
		EXPECT_TRUE(_voice->isPlayingNote(69));

		_buffer.fill(eden::SampleType(0));
		_voice->renderBlock(_buffer, 0, BUFFER_LENGTH);

		const auto detectedFrequency = TestUtils::detectFrequency(_buffer.getReadPointer(0), BUFFER_LENGTH, SAMPLE_RATE);
		EXPECT_NEAR(440.0, detectedFrequency, 0.5f);

		_voice->stopNote(0.f);

		_buffer.fill(eden::SampleType(0));
		_voice->renderBlock(_buffer, 0, BUFFER_LENGTH);

		// silence should start from sample 48, but starts from sample 54
		for (auto i = 54u; i < BUFFER_LENGTH; ++i)
		{
			EXPECT_FLOAT_EQ(_buffer.getReadPointer(0)[i], eden::SampleType(0));
		}
	}

	TEST_F(VoiceTest, CalculatePitch)
	{
		EXPECT_DOUBLE_EQ(_voice->calculatePitch(69, 0), 440.0);
	}

	TEST_F(VoiceTest, VariousParameters)
	{
		constexpr auto sampleRate = 1000.0;
		constexpr auto blockLength = 250u;
		constexpr auto segmentTime = 250ms;
		constexpr eden::SampleType breakLevel(0.5);
		constexpr int noteNumber = 83;

		_voice->setSampleRate(sampleRate);
		_voice->setBlockLength(250u);
		_voice->setEnvelope(std::make_unique<eden::synth::envelope::ADBDR>(sampleRate, eden::ADBDRParameters(segmentTime, eden::EnvelopeSegmentCurve::Linear, segmentTime, eden::EnvelopeSegmentCurve::Linear, segmentTime, eden::EnvelopeSegmentCurve::Linear, segmentTime, eden::EnvelopeSegmentCurve::Linear, breakLevel)));
		_voice->setWaveTable({ 1.f, 1.f });

		EXPECT_DOUBLE_EQ(_voice->getSampleRate(), sampleRate);
		EXPECT_FALSE(_voice->isPlaying());

		_voice->startNote(noteNumber, 1.0f);

		EXPECT_TRUE(_voice->isPlaying());
		EXPECT_TRUE(_voice->isPlayingNote(noteNumber));

		_buffer = eden::AudioBuffer(1, blockLength);

		// attack
		_buffer.fill(eden::SampleType(0));
		_voice->renderBlock(_buffer, 0, blockLength);

		EXPECT_NEAR(_buffer.getReadPointer(0)[blockLength - 1], _voice->gainValue() * 1.0f, 0.05f);

		// decay1
		_buffer.fill(eden::SampleType(0));
		_voice->renderBlock(_buffer, 0, blockLength);

		EXPECT_NEAR(_buffer.getReadPointer(0)[blockLength - 1], _voice->gainValue() * breakLevel, 0.05f);

		// release
		_voice->stopNote(0.f);

		_buffer.fill(eden::SampleType(0));
		_voice->renderBlock(_buffer, 0, blockLength);

		EXPECT_NEAR(_buffer.getReadPointer(0)[blockLength - 1], 0.f, 0.05f);

		// silence
		_buffer.fill(eden::SampleType(0));
		_voice->renderBlock(_buffer, 0, blockLength);

		for (auto i = 1u; i < blockLength; ++i)
		{
			EXPECT_FLOAT_EQ(_buffer.getReadPointer(0)[i], eden::SampleType(0));
		}

		EXPECT_FALSE(_voice->isPlayingNote(noteNumber));
		EXPECT_FALSE(_voice->isPlaying());
	}
}
