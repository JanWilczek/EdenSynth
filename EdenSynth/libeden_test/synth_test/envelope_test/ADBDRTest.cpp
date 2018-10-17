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
		const unsigned NUM_SAMPLES = 10000;
		const double SAMPLE_RATE = 48000.0;
		std::unique_ptr<eden::SampleType[]> _channel = std::make_unique<eden::SampleType[]>(NUM_SAMPLES);

		void SetUp() override
		{
			for (auto i = 0u; i < NUM_SAMPLES; ++i)
			{
				_channel[i] = eden::SampleType(1.0);
			}
		}

		void processAttack(eden::synth::envelope::Envelope& envelope, unsigned& attackEndSample)
		{
			bool firstIteration = true;

			while (attackEndSample > NUM_SAMPLES - 1)
			{
				envelope.apply(_channel.get(), 0, NUM_SAMPLES);
				if (firstIteration)
				{
					EXPECT_NEAR(_channel[0], eden::SampleType(0.0), 1e-3);
					firstIteration = false;
				}
				attackEndSample -= NUM_SAMPLES;
			}
			envelope.apply(_channel.get(), 0, attackEndSample + 1);

			if (firstIteration)
			{
				EXPECT_NEAR(_channel[0], eden::SampleType(0.0), 1e-3);
				firstIteration = false;
			}

			EXPECT_NEAR(_channel[attackEndSample], eden::SampleType(1.0), 1e-3);
		}

		void shapeTest(unsigned startSample, unsigned endSample)
		{
			// test for exponential shape of the envelope
			const unsigned middleSample = static_cast<unsigned>((endSample - startSample) / 2) + startSample;
			const auto startSampleLevel = 20 * std::log10(_channel[startSample]);
			const auto middleSampleLevel = 20 * std::log10(_channel[middleSample]);
			const auto lastSampleLevel = 20 * std::log10(_channel[endSample - 1]);

			EXPECT_NEAR(middleSampleLevel, (startSampleLevel + lastSampleLevel) / 2, 0.2);
		}

		template<typename Predicate>
		unsigned findFirstSample(unsigned startSample, Predicate&& predicate)
		{
			for (auto i = startSample; i < NUM_SAMPLES; ++i)
			{
				if (predicate(_channel[i]))
				{
					return i;
				}
			}
			return 0u;
		}
	};

	/* Envelopes can be tested against:
	* 1. Shape
	* 2. Duration
	* 3. Levels of break points.
	*/

	TEST_P(ADBDRTest, AttackDecayTest)
	{
		const auto data = GetParam();
		eden::synth::envelope::ADBDR envelope(SAMPLE_RATE, data.attackTime,
			data.decay1Time, data.decay2Time, data.releaseTime, data.breakLevel);

		envelope.keyOn();

		const auto attackSamples = eden::utility::TimeSampleConverter::timeToSamples(data.attackTime, SAMPLE_RATE) + 1;
		auto attackEndSample = attackSamples;
		
		processAttack(envelope, attackEndSample);
		
		ASSERT_TRUE(attackEndSample < NUM_SAMPLES - 1);
		envelope.apply(_channel.get(), attackEndSample + 1, NUM_SAMPLES - attackEndSample - 1);
		//auto breakSample = findFirstSample(attackEndSample + 1, [&](eden::SampleType sample) {return std::abs(sample - data.breakLevel) < 1e-2; });
		auto breakSample = eden::utility::TimeSampleConverter::timeToSamples(data.decay1Time, SAMPLE_RATE) + attackEndSample;

		ASSERT_TRUE(breakSample < NUM_SAMPLES);
		EXPECT_NEAR(_channel[breakSample], data.breakLevel, 1e-2);

		const auto decay1Time = eden::utility::TimeSampleConverter::samplesToMilliseconds(breakSample, SAMPLE_RATE) - data.attackTime;
		EXPECT_NEAR(decay1Time.count(), data.decay1Time.count(), 1);
		
		shapeTest(breakSample, NUM_SAMPLES - 1);
	}

	TEST_P(ADBDRTest, ReleaseTest)
	{
		const auto data = GetParam();
		eden::synth::envelope::ADBDR envelope(SAMPLE_RATE, data.attackTime,
			data.decay1Time, data.decay2Time, data.releaseTime, data.breakLevel);

		envelope.keyOn();
		envelope.apply(_channel.get(), 0, NUM_SAMPLES - 1);

		const auto attackSamples = eden::utility::TimeSampleConverter::timeToSamples(data.attackTime, SAMPLE_RATE) + 1;
		auto attackEndSample = attackSamples;
		processAttack(envelope, attackEndSample);
		envelope.keyOff();
		
		// TODO: The break point may be behind envelope's end.
		auto breakSample = findFirstSample(attackEndSample, [&](eden::SampleType sample) {return std::abs(sample - data.breakLevel) < 1e-2; });

		envelope.apply(_channel.get(), breakSample, NUM_SAMPLES - 1 - breakSample);

		// TODO: Here may be mistakes.
		auto finalSample = 0u;
		for (auto i = attackSamples; i < NUM_SAMPLES; ++i)
		{
			if (_channel[i] == 0.0)
			{
				finalSample = i;
				break;
			}
		}
		if (finalSample == 0u)
		{
			finalSample = NUM_SAMPLES - 1;
		}

		const auto releaseTime = eden::utility::TimeSampleConverter::samplesToMilliseconds(finalSample + 1, SAMPLE_RATE);
		EXPECT_NEAR(releaseTime.count(), data.releaseTime.count(), 5);

		shapeTest(breakSample, finalSample);
	}

	constexpr ADBDRTestData testData[] =
	{
		{ 10ms, 30ms, 400ms, 100ms, eden::SampleType(0.6)},
		{ 200ms, 50ms, 10000ms, 300ms, eden::SampleType(0.9)},
		{ 1000ms, 100ms, 5000ms, 1000ms, eden::SampleType(0.8)},
	};

	INSTANTIATE_TEST_CASE_P(Envelope, ADBDRTest, ::testing::ValuesIn(std::begin(testData), std::end(testData)));
}
