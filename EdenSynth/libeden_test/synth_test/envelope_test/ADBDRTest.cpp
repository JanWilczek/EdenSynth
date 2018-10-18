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
		std::unique_ptr<eden::synth::envelope::Envelope> _envelope;
		ADBDRTestData _data;

		void SetUp() override
		{
			fillChannel(eden::SampleType(1.0));
			_data = GetParam();
			_envelope = std::make_unique<eden::synth::envelope::ADBDR>(SAMPLE_RATE, _data.attackTime,
				_data.decay1Time, _data.decay2Time, _data.releaseTime, _data.breakLevel);
		}

		void fillChannel(const eden::SampleType value)
		{
			for (auto i = 0u; i < NUM_SAMPLES; ++i)
			{
				_channel[i] = eden::SampleType(1.0);
			}
		}

		void processSamplesRange(const unsigned fromSample, unsigned& toSample)
		{
			ASSERT_LE(fromSample, toSample);

			if (toSample < NUM_SAMPLES)
			{
				_envelope->apply(_channel.get(), fromSample, toSample - fromSample + 1);
			}
			else
			{
				_envelope->apply(_channel.get(), fromSample, NUM_SAMPLES - fromSample);
				toSample -= NUM_SAMPLES;
				while (toSample > NUM_SAMPLES - 1)
				{
					fillChannel(eden::SampleType(1.0));
					_envelope->apply(_channel.get(), 0, NUM_SAMPLES);
					toSample -= NUM_SAMPLES;
				}
				fillChannel(eden::SampleType(1.0));
				_envelope->apply(_channel.get(), 0, toSample + 1);
			}
		}

		void shapeTest(unsigned startSample, unsigned endSample)
		{
			ASSERT_LT(startSample, endSample);

			// test for exponential shape of the envelope
			const unsigned middleSample = static_cast<unsigned>((endSample - startSample) / 2 + 1) + startSample;
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
		_envelope->keyOn();

		const auto attackSamples = eden::utility::TimeSampleConverter::timeToSamples(_data.attackTime, SAMPLE_RATE) + 1;
		auto attackEndSample = attackSamples;

		processSamplesRange(0, attackEndSample);
		ASSERT_TRUE(attackEndSample < NUM_SAMPLES - 1);
		EXPECT_NEAR(_channel[attackEndSample], eden::SampleType(1.0), 2e-3);

		const auto decay1Samples = eden::utility::TimeSampleConverter::timeToSamples(_data.decay1Time, SAMPLE_RATE);
		auto breakSample = decay1Samples + attackEndSample;
		processSamplesRange(attackEndSample + 1, breakSample);

		ASSERT_TRUE(breakSample < NUM_SAMPLES);
		EXPECT_NEAR(_channel[breakSample], _data.breakLevel, 1e-2);

		auto endChannel = NUM_SAMPLES - 1;
		processSamplesRange(breakSample + 1, endChannel);
		const auto endSample = findFirstSample(breakSample, [](eden::SampleType sample) { return std::abs(sample - eden::SampleType(0.0)) < 1e-6; });

		shapeTest(breakSample, endSample > 0u ? endSample : NUM_SAMPLES - 1);
	}

	TEST_P(ADBDRTest, ReleaseTest)
	{
		_envelope->keyOn();

		const auto attackDecay1Samples = eden::utility::TimeSampleConverter::timeToSamples(_data.attackTime + _data.decay1Time, SAMPLE_RATE) + 1;
		auto decay1EndSample = attackDecay1Samples;
		processSamplesRange(0, decay1EndSample);
		_envelope->keyOff();

		const auto releaseSamples = eden::utility::TimeSampleConverter::timeToSamples(_data.releaseTime, SAMPLE_RATE);
		auto endSample = releaseSamples + decay1EndSample;
		processSamplesRange(decay1EndSample, endSample);
		auto endChannel = NUM_SAMPLES - 1;
		processSamplesRange(endSample, endChannel);

		// Handle case when release is very short (only one apply iteration) and when it is very long (we can start from sample 0 then).
		const auto smallestPossibleReleaseStart = decay1EndSample < endSample ? decay1EndSample : 0u;
		auto foundEndSample = findFirstSample(smallestPossibleReleaseStart, [](eden::SampleType sample) { return std::abs(sample - eden::SampleType(0.0)) < 1e-6; });
		EXPECT_LE(foundEndSample, endSample);

		shapeTest(smallestPossibleReleaseStart, foundEndSample);
	}

	constexpr ADBDRTestData testData[] =
	{
		{ 10ms, 30ms, 400ms, 100ms, eden::SampleType(0.6)},
		{ 200ms, 50ms, 10000ms, 300ms, eden::SampleType(0.9)},
		{ 1000ms, 100ms, 5000ms, 1000ms, eden::SampleType(0.8)},
	};

	INSTANTIATE_TEST_CASE_P(Envelope, ADBDRTest, ::testing::ValuesIn(std::begin(testData), std::end(testData)));
}
