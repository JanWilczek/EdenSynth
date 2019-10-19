/// 
/// \author Jan Wilczek
/// \date 12.10.2019
/// 
#include "pch.h"
#include "synth/wavetable/WhiteNoiseSource.h"

#include "fftw++.h"

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

	TEST_F(WhiteNoiseSourceTest, Clipping)
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

	TEST_F(WhiteNoiseSourceTest, ZeroMean)
	{
		// Given the noise generator

		// When it is playing
		constexpr auto bufferLength = 480000;
		auto sum = 0.f;
		for (auto i = 0; i < bufferLength; ++i)
		{
			sum += _whiteNoiseGenerator.getSample();
		}

		// Then the mean of the output should be around 0
		const auto mean = sum / static_cast<float>(bufferLength);
		ASSERT_NEAR(0.f, mean, 1e-2f);
	}

	TEST_F(WhiteNoiseSourceTest, EqualPowerInLinearFrequencyBands)
	{
		// Given the noise generator

		// When it is playing
		constexpr auto bufferLength = 480u;
		double samplesBuffer[bufferLength];
		for (auto i = 0; i < bufferLength; ++i)
		{
			samplesBuffer[i] = _whiteNoiseGenerator.getSample();
		}

		// Then the power across the linear frequency bands should be more or less equal
		constexpr auto fftLength = bufferLength / 2 + 1;	// size according to the FFTW++ library
		Complex samplesBufferFft[fftLength];
		fftwpp::rcfft1d fft(bufferLength, samplesBuffer, samplesBufferFft);
		fft.fft(samplesBuffer, samplesBufferFft);
	}
}
