/// 
/// \author Jan Wilczek
/// \date 06.10.2018
/// 
#include "pch.h"
#include <eden/AudioBuffer.h>
#include <synth/Synthesiser.h>
#include "eden/MidiBuffer.h"
#include "settings/Settings.h"

namespace libeden_test
{
	class SynthesiserTest : public ::testing::Test
	{
	protected:
		void SetUp() override
		{
			audioBuffer.fill(0);
		}

		const double SAMPLE_RATE = 48000;
		eden::settings::Settings _settings{};
		eden::synth::Synthesiser synthesiser{_settings};
		const int NUM_CHANNELS = 2;
		const unsigned BUFFER_LENGTH = 1024;
		eden::AudioBuffer audioBuffer{ NUM_CHANNELS, BUFFER_LENGTH };
		eden::MidiBuffer midiBuffer;
	};

	TEST_F(SynthesiserTest, ZeroInZeroOut)
	{
		synthesiser.processBlock(audioBuffer, midiBuffer, BUFFER_LENGTH);

		audioBuffer.forEachChannel([&](float* channel)
		{
			for (auto i = 0u; i < BUFFER_LENGTH; ++i)
			{
				EXPECT_FLOAT_EQ(channel[i], 0.f);
			}
		});
	}
}
