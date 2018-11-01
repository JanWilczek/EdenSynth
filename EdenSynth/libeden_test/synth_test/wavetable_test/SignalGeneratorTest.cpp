/// 
/// \author Jan Wilczek
/// \date 11.01.2018
/// 
#include "pch.h"
#include "synth/wavetable/SignalGenerator.h"

namespace libeden_test
{
	using SignalGenerator = eden::synth::wavetable::SignalGenerator;

	class SignalGeneratorTest : public ::testing::Test
	{
	protected:
		void SetUp() override
		{
			_signalGenerator = std::make_unique<SignalGenerator>(SAMPLE_RATE);
			
			for (auto i = 0u; i < CHANNEL_LENGTH; ++i)
			{
				_audioChannel[i] = eden::SampleType(0);
			}
		}

		const double SAMPLE_RATE = 48000.0;
		constexpr static unsigned CHANNEL_LENGTH = 480u;
		eden::SampleType _audioChannel[CHANNEL_LENGTH];
		std::unique_ptr<eden::synth::wavetable::SignalGenerator> _signalGenerator;
	};

	TEST_F(SignalGeneratorTest, ZeroInZeroOut)
	{
		_signalGenerator->generateSignal(_audioChannel, 0, CHANNEL_LENGTH);

		for (auto i = 0u; i < CHANNEL_LENGTH; ++i)
		{
			EXPECT_FLOAT_EQ(_audioChannel[i], eden::SampleType(0));
		}
	}
}
