/// 
/// \author Jan Wilczek
/// \date 16.10.18
/// 
#include "pch.h"
#include "synth/envelope/ADBDR.h"
#include "utility/TimeSampleConverter.h"
#include "eden/EnvelopeParameters.h"

namespace libeden_test
{
	using ms = std::chrono::milliseconds;

	struct ADBDRTestData
	{
		ms attackTime;
		ms decay1Time;
		ms decay2Time;
		ms releaseTime;
		float breakLevel;
	};

	class ADBDRTest : public ::testing::TestWithParam<ADBDRTestData>
	{
	protected:
		const unsigned NUM_SAMPLES = 10000;
		const double SAMPLE_RATE = 48000.0;
		std::unique_ptr<float[]> _channel = std::make_unique<float[]>(NUM_SAMPLES);
		std::unique_ptr<eden::synth::envelope::Envelope> _envelope;
		ADBDRTestData _data;

		void SetUp() override
		{
			fillChannel(1.f);
			_data = GetParam();
			eden::ADBDRParameters envelopeParameters( _data.attackTime, eden::EnvelopeSegmentCurve::Exponential,
				_data.decay1Time, eden::EnvelopeSegmentCurve::Exponential,
				_data.decay2Time, eden::EnvelopeSegmentCurve::Exponential,
				_data.releaseTime, eden::EnvelopeSegmentCurve::Exponential,
				_data.breakLevel);
			_envelope = std::make_unique<eden::synth::envelope::ADBDR>(SAMPLE_RATE, envelopeParameters);
		}

		void fillChannel(const float value)
		{
			fillChannel(value, 0, NUM_SAMPLES - 1);
		}

		void fillChannel(const float value, unsigned fromSample, unsigned toSample)
		{
			for (; fromSample <= toSample; ++fromSample)
			{
				_channel[fromSample] = value;
			}
		}

		/// <summary>
		/// Applies envelope to samples in [fromSample, toSample] range.
		/// </summary>
		/// <param name="fromSample"></param>
		/// <param name="toSample">after execution this parameter is the index in buffer of the last processed sample</param>
		/// <returns>the number of processed samples</returns>
		unsigned processSamplesRange(const unsigned fromSample, unsigned& toSample)
		{
			if (toSample < fromSample)
			{
				return 0u;
			}

			if (toSample < NUM_SAMPLES)
			{
				const auto samplesToProcess = toSample - fromSample + 1;
				fillChannel(float(1.0), fromSample, toSample);
				_envelope->apply(_channel.get(), fromSample, samplesToProcess);
				return samplesToProcess;
			}

			auto processedSamples = 0u;
			const auto initialSamplesToProcess = NUM_SAMPLES - fromSample;

			fillChannel(float(1.0), fromSample, NUM_SAMPLES - 1);
			_envelope->apply(_channel.get(), fromSample, initialSamplesToProcess);

			processedSamples += initialSamplesToProcess;
			toSample -= NUM_SAMPLES;

			while (toSample > NUM_SAMPLES - 1)
			{
				fillChannel(float(1.0));
				_envelope->apply(_channel.get(), 0, NUM_SAMPLES);

				processedSamples += NUM_SAMPLES;
				toSample -= NUM_SAMPLES;
			}
			const auto finalSamplesToProcess = toSample + 1;

			fillChannel(float(1.0), 0, finalSamplesToProcess - 1);
			_envelope->apply(_channel.get(), 0, finalSamplesToProcess);
			processedSamples += finalSamplesToProcess;

			return processedSamples;
		}

		void shapeTest(int startSample, int endSample)
		{
			if (endSample < startSample)
			{
				return;
			}

			// test for exponential shape of the envelope
			const unsigned middleSample = static_cast<unsigned>((endSample + startSample) / 2);
			const auto startSampleLevel = 20 * std::log10(_channel[startSample]);
			const auto middleSampleLevel = 20 * std::log10(_channel[middleSample]);
			const auto lastSampleLevel = 20 * std::log10(_channel[endSample]);

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
		EXPECT_NEAR(_channel[attackEndSample], float(1.0), 2e-3);

		const auto decay1Samples = eden::utility::TimeSampleConverter::timeToSamples(_data.decay1Time, SAMPLE_RATE);
		auto breakSample = decay1Samples + attackEndSample;
		processSamplesRange(attackEndSample + 1, breakSample);

		ASSERT_TRUE(breakSample < NUM_SAMPLES);
		EXPECT_NEAR(_channel[breakSample], _data.breakLevel, 1e-2);

		auto endChannel = NUM_SAMPLES - 1;
		processSamplesRange(breakSample + 1, endChannel);
		const auto endSample = findFirstSample(breakSample, [](float sample) { return std::abs(sample - float(0.0)) < 1e-6; });

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
		auto processedSamples = processSamplesRange(decay1EndSample, endSample);
		auto endChannel = NUM_SAMPLES - endSample;
		processedSamples += processSamplesRange(endSample + 1, endChannel);

		EXPECT_FLOAT_EQ(_channel[endSample], 0.0f);

		// Handle case when release is very short (only one apply iteration) and when it is very long (we can start from sample 0 then).
		unsigned releaseStartOnChannel;
		if (processedSamples > NUM_SAMPLES - decay1EndSample)
		{
			releaseStartOnChannel = 0u;
		}
		else
		{
			releaseStartOnChannel = decay1EndSample;
		}

		auto foundEndSample = findFirstSample(releaseStartOnChannel, [](float sample) { return std::abs(sample - float(0.0)) < 1e-6; });
		EXPECT_LE(foundEndSample, endSample);

		shapeTest(releaseStartOnChannel, foundEndSample - 1);
	}

	constexpr ADBDRTestData testData[] =
	{
		{ 10ms, 30ms, 400ms, 100ms, float(0.6)},
		{ 200ms, 50ms, 10000ms, 300ms, float(0.9)},
		{ 1000ms, 100ms, 5000ms, 1000ms, float(0.8)},
	};

	INSTANTIATE_TEST_CASE_P(Envelope, ADBDRTest, ::testing::ValuesIn(std::begin(testData), std::end(testData)));
}
