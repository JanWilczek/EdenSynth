/// 
/// \author Jan Wilczek
/// \date 11.01.2018
/// 
#include "pch.h"
#include "synth/wavetable/SignalGenerator.h"
#include "synth/wavetable/WaveTableSource.h"

namespace libeden_test
{
	using SignalGenerator = eden::synth::wavetable::SignalGenerator;

	class SignalGeneratorTest : public ::testing::Test
	{
	protected:
		void SetUp() override
		{
			_signalGenerator = std::make_unique<SignalGenerator>();
			_signalGenerator->addOscillator(eden::synth::wavetable::SynthOscillator{ _oscillatorId , std::make_unique<eden::synth::wavetable::WaveTableSource>(SAMPLE_RATE) });
			_signalGenerator->setSampleRate(SAMPLE_RATE);

			for (auto i = 0u; i < CHANNEL_LENGTH; ++i)
			{
				_audioChannel[i] = eden::SampleType(0);
			}
		}

		const double SAMPLE_RATE = 48000.0;
		constexpr static unsigned CHANNEL_LENGTH = 480u;
		eden::SampleType _audioChannel[CHANNEL_LENGTH];

		constexpr static eden::OscillatorId _oscillatorId = 0u;
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

	TEST_F(SignalGeneratorTest, RangeAndStopTest)
	{
		_signalGenerator->setPitch(440.0);
		constexpr auto start = 100;
		constexpr auto length = 100;
		_signalGenerator->generateSignal(_audioChannel, start, length);

		// not processed samples and the first processed sample should be 0
		for (auto i = 0u; i <= start; ++i)
		{
			EXPECT_FLOAT_EQ(_audioChannel[i], eden::SampleType(0));
		}

		// processed samples beyond the first should be different than 0
		for (auto i = start + 1; i < start + length; ++i)
		{
			EXPECT_NE(_audioChannel[i], eden::SampleType(0));
		}

		// not processed samples at the end of the buffer should be 0
		for (auto i = start + length; i < CHANNEL_LENGTH; ++i)
		{
			EXPECT_FLOAT_EQ(_audioChannel[i], eden::SampleType(0));
		}

		_signalGenerator->stop();
		_signalGenerator->generateSignal(_audioChannel, 0, CHANNEL_LENGTH);

		for (auto i = 0u; i < CHANNEL_LENGTH; ++i)
		{
			EXPECT_FLOAT_EQ(_audioChannel[i], eden::SampleType(0));
		}
	}

	TEST_F(SignalGeneratorTest, WaveTableManipulation)
	{
		const std::vector<eden::SampleType> waveTable = { eden::SampleType(1.0), eden::SampleType(0.0) };
		auto source = std::make_unique<eden::synth::wavetable::WaveTableSource>(SAMPLE_RATE);
		source->setWaveTable(waveTable);
		_signalGenerator->setOscillatorSource(_oscillatorId, std::move(source));
		_signalGenerator->setSampleRate(10.0);
		_signalGenerator->setPitch(5.0);
		_signalGenerator->generateSignal(_audioChannel, 0, 10);
		
		for (auto i = 0u; i < 10; ++i)
		{
			EXPECT_FLOAT_EQ(_audioChannel[i], eden::SampleType(1 - (i % 2)));
		}
	}

	TEST_F(SignalGeneratorTest, RoughFrequencyTest)
	{
		_signalGenerator->setPitch(100.0);
		_signalGenerator->generateSignal(_audioChannel, 0, CHANNEL_LENGTH);

		EXPECT_FLOAT_EQ(_audioChannel[0], eden::SampleType(0));
		EXPECT_LT(_audioChannel[CHANNEL_LENGTH - 1], eden::SampleType(0));

		auto nbZeroCrossings = 0u;

		for (int i = 1; i < CHANNEL_LENGTH; ++i)
		{
			if (_audioChannel[i] * _audioChannel[i-1] < 0.f)
			{
				++nbZeroCrossings;
			}
		}

		EXPECT_EQ(nbZeroCrossings, 1u);
	}
}
