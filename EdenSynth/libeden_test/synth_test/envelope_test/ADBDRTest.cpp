/// 
/// \author Jan Wilczek
/// \date 16.10.18
/// 
#include "pch.h"
#include "synth/envelope/ADBDR.h"
#include "utility/TimeSampleConverter.h"

namespace libeden_test
{
	using ms = std::chrono::milliseconds;

	struct ADBDRTestData
	{
		ms attackTime;
		ms decay1Time;
		ms decay2Time;
		ms releaseTime;
		eden::SampleType breakLevel;
	};

	class ADBDRTest : public ::testing::TestWithParam<ADBDRTestData>
	{
	protected:
		const double sampleRate = 48000.0;
	};

	TEST_P(ADBDRTest, TimeTest)
	{
		const auto data = GetParam();
		eden::synth::envelope::ADBDR envelope(sampleRate, data.attackTime,
			data.decay1Time, data.decay2Time, data.releaseTime, data.breakLevel);

		/* With envelopes we can test:
		 * 1. Shape
		 * 2. Duration
		 * 3. Levels of break points.
		 */

		envelope.keyOn();

		constexpr unsigned NUM_SAMPLES = 10000;
		auto channel = std::make_unique<eden::SampleType[]>(NUM_SAMPLES);
		for (auto i = 0u; i < NUM_SAMPLES; ++i)
		{
			channel[i] = eden::SampleType(1.0);
		}

		envelope.apply(channel.get(), 0, NUM_SAMPLES);

		const auto attackSamples = eden::utility::TimeSampleConverter::timeToSamples(data.attackTime, sampleRate) + 1;

		EXPECT_NEAR(channel[0], eden::SampleType(0.0), 1e-3);
		EXPECT_NEAR(channel[attackSamples], eden::SampleType(1.0), 1e-3);

		auto breakSample = 0u;
		for (auto i = attackSamples; i < NUM_SAMPLES; ++i)
		{
			if (std::abs(channel[i] - data.breakLevel) < 1e-2)
			{
				breakSample = i;
				break;
			}
		}

		const auto decay1Time = eden::utility::TimeSampleConverter::samplesToMilliseconds(breakSample, sampleRate) - data.attackTime;
		EXPECT_NEAR(decay1Time.count(), data.decay1Time.count(), 1);
	}

	constexpr ADBDRTestData testData[] =
	{
		{ 10ms, 30ms, 400ms, 100ms, eden::SampleType(0.6)}
	};

	INSTANTIATE_TEST_CASE_P(Envelope, ADBDRTest, ::testing::ValuesIn(std::begin(testData), std::end(testData)));
}
