/// 
/// \author Jan Wilczek
/// \date 12.10.2019
/// 
#include "pch.h"
#include "synth/wavetable/WhiteNoiseSource.h"

/*
 * Test cases:
 * 1. Is output produced in range [-1, 1)? // done
 * 2. Mean should be around 0
 * 3. Uniform power across all frequency ranges (in linear scale) / Rising power with frequency in tertial frequency bands
 * 4. Invariable to pitch
 */

namespace libeden_test
{
	using WhiteNoiseSource = eden::synth::wavetable::WhiteNoiseSource;

	class WhiteNoiseSourceTest : public ::testing::Test
	{
	protected:
		void SetUp() override
		{
			_whiteNoiseGenerator = WhiteNoiseSource();
		}

		WhiteNoiseSource _whiteNoiseGenerator;
	};

	TEST_F(WhiteNoiseSourceTest, RangeTest)
	{
		// Given the noise generator

		// When it is playing

		// Then it should produce output in [-1,1] range
		constexpr auto bufferLength = 480;
		for (auto i = 0; i < bufferLength; ++i)
		{
			const auto sample = _whiteNoiseGenerator.getSample();

			ASSERT_GE(sample, -1.f);
			ASSERT_LE(sample, 1.f);
		}

	}
}
