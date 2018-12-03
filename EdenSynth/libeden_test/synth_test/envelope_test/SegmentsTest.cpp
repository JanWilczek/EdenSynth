/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include "pch.h"
#include <vector>
#include "synth/envelope/Attack.h"
#include "synth/envelope/Decay.h"
#include "synth/envelope/Sustain.h"
#include "synth/envelope/LinearGain.h"
#include "synth/envelope/ExponentialGain.h"
#include "utility/TimeSampleConverter.h"

namespace libeden_test
{
	struct SegmentTestParam
	{
		std::chrono::milliseconds duration;
		float initialLevel;
		float finalLevel;
	};

	class SegmentTest : public ::testing::Test
	{
	protected:
		const float SAMPLE_RATE = 48000.f;

		void testSegment(eden::synth::envelope::EnvelopeSegment& segment, SegmentTestParam testData, double durationTolerance, float& lastLevel)
		{
			unsigned iterationCount = 0u;
			float currentLevel = testData.initialLevel;

			while (!segment.hasEnded(currentLevel))
			{
				segment.applyAndUpdateGain(currentLevel);
				++iterationCount;
			}

			const auto realDuration = eden::utility::TimeSampleConverter::samplesToMilliseconds(iterationCount, SAMPLE_RATE);
			EXPECT_NEAR(testData.duration.count(), realDuration.count(), durationTolerance);
			EXPECT_NEAR(currentLevel, testData.finalLevel, 1e-3);

			lastLevel = currentLevel;
		}
	};

	class AttackTest : public SegmentTest, public ::testing::WithParamInterface<SegmentTestParam>
	{
	};

	TEST_P(AttackTest, LinearGain)
	{
		float lastLevel;
		auto data = GetParam();

		eden::synth::envelope::Attack segmentLinear(SAMPLE_RATE, std::make_unique<eden::synth::envelope::LinearGain>(), data.duration, data.initialLevel, data.finalLevel);
		testSegment(segmentLinear, data, 0.015 * static_cast<double>(data.duration.count()), lastLevel);
		EXPECT_LE(lastLevel, data.finalLevel + 1e-3);
	}

	TEST_P(AttackTest, ExponentialGain)
	{
		float lastLevel;
		auto data = GetParam();

		eden::synth::envelope::Attack segmentExponential(SAMPLE_RATE, std::make_unique<eden::synth::envelope::ExponentialGain>(), data.duration, data.initialLevel, data.finalLevel);
		testSegment(segmentExponential, data, 0.02 * static_cast<double>(data.duration.count()), lastLevel);
		EXPECT_LE(lastLevel, data.finalLevel + 1e-3);
	}

	const std::vector<SegmentTestParam> attackTestParameters =
	{
		{ 480ms, float(0.0), float(1.0)},
		{ 10ms, float(0.0), float(1.0)},
		{ 100ms, float(0.1), float(0.9)},
		{1000ms, float(0.4), float(0.7)},
		{10000ms, float(0.33), float(1.0)}
	};

	INSTANTIATE_TEST_CASE_P(Usage, AttackTest, ::testing::ValuesIn(attackTestParameters));

	class DecayTest : public SegmentTest, public ::testing::WithParamInterface<SegmentTestParam>
	{
	};

	TEST_P(DecayTest, LinearGain)
	{
		float lastLevel;
		auto data = GetParam();

		eden::synth::envelope::Decay segmentLinear(SAMPLE_RATE, std::make_unique<eden::synth::envelope::LinearGain>(), data.duration, data.initialLevel, data.finalLevel);
		testSegment(segmentLinear, data, 0.015 * static_cast<double>(data.duration.count()), lastLevel);
		EXPECT_GE(lastLevel, data.finalLevel - 1e-3);
	}

	TEST_P(DecayTest, ExponentialGain)
	{
		float lastLevel;
		auto data = GetParam();

		eden::synth::envelope::Decay segmentExponential(SAMPLE_RATE, std::make_unique<eden::synth::envelope::ExponentialGain>(), data.duration, data.initialLevel, data.finalLevel);
		testSegment(segmentExponential, data, 0.15 * static_cast<double>(data.duration.count()), lastLevel);
		EXPECT_GE(lastLevel, data.finalLevel - 1e-3);
	}

	const std::vector<SegmentTestParam> decayTestParameters =
	{
		{ 250ms, float(1.0), float(0.0)},
		{ 50ms, float(1.0), float(0.0)},
		{ 1000ms, float(0.8), float(0.3)},
		{20000ms, float(0.5), float(0.0)}
	};

	INSTANTIATE_TEST_CASE_P(Usage, DecayTest, ::testing::ValuesIn(decayTestParameters));

	TEST(SegmentsTest, Sustain)
	{
		constexpr float levels[] = { 0.44f, 0.33f, 0.6f, 0.55f, 0.98f, 1.0f, 0.0f, 0.0001f };

		eden::synth::envelope::Sustain sustain{};
		for (auto level : levels)
		{
			const auto oldLevel = level;
			sustain.applyAndUpdateGain(level);

			EXPECT_FLOAT_EQ(oldLevel, level);
			EXPECT_FALSE(sustain.hasEnded(level));
		}
	}
}
