/// 
/// \author Jan Wilczek
/// \date 02.11.2018
/// 
#include "pch.h"
#include <synth/Voice.h>
#include <eden/AudioBuffer.h>
#include "TestUtils.h"

namespace libeden_test
{
	class VoiceTest : public ::testing::Test
	{
	protected:
		void SetUp() override
		{
			_voice = std::make_unique<eden::synth::Voice>(SAMPLE_RATE);
			_buffer.fill(eden::SampleType(0));
		}

		const double SAMPLE_RATE = 48000.0;
		std::unique_ptr<eden::synth::Voice> _voice;

		constexpr static unsigned BUFFER_LENGTH = 480u;
		eden::AudioBuffer _buffer{1, BUFFER_LENGTH};
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

		_voice->startNote(noteNumber, 1.0f);
		_voice->renderBlock(_buffer, 0, BUFFER_LENGTH);

		const auto detectedFrequency = TestUtils::calculateFrequency(_buffer.getReadPointer(0), BUFFER_LENGTH, SAMPLE_RATE);
		EXPECT_NEAR(440.0, detectedFrequency, 0.5f);
	}
}
