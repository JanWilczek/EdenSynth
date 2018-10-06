/// 
/// \author Jan Wilczek
/// \date 05.10.2018
/// 
#include "pch.h"

#include <eden/AudioBuffer.h>

namespace libeden_test
{
	TEST(AudioBufferTest, PassMemoryBlock)
	{
		constexpr int numChannels = 2;
		eden::AudioBuffer::SampleType** data = new eden::AudioBuffer::SampleType*[numChannels];

		constexpr unsigned numSamples = 1024;
		for (int i = 0; i < numChannels; ++i)
		{
			data[i] = new eden::AudioBuffer::SampleType[numSamples];
		}

		for (int i = 0; i < numChannels; ++i)
		{
			for (int j = 0; j < numSamples; ++j)
			{
				data[i][j] = 0;
			}
		}

		eden::AudioBuffer testBuffer(data, numChannels, numSamples);

		ASSERT_EQ(testBuffer.getNumChannels(), numChannels);
		ASSERT_EQ(testBuffer.getNumSamples(), numSamples);

		for (int i = 0; i < testBuffer.getNumChannels(); ++i)
		{
			for (unsigned j = 0; j < testBuffer.getNumSamples(); ++j)
			{
				EXPECT_FLOAT_EQ(testBuffer.getArrayOfWritePointers()[i][j], 0);
			}
		}

		for (int i = 0; i < numChannels; ++i)
		{
			delete[] data[i];
		}
		delete[] data;
	}

	TEST(AudioBufferTest, AllocationDeallocation)
	{
		constexpr int numCases = 6;
		constexpr std::array<int, numCases> numChannels{ 2, 5, 0, 1, 10, 3 };
		constexpr std::array<unsigned, numCases> numSamples{ 2048, 512, 234, 340, 48000, 96000 };
		for (int i = 0; i < numCases; ++i)
		{
			eden::AudioBuffer testBuffer(numChannels[i], numSamples[i]);
			EXPECT_EQ(testBuffer.getNumChannels(), numChannels[i]);
			EXPECT_EQ(testBuffer.getNumSamples(), numSamples[i]);
		}

		ASSERT_NO_THROW(eden::AudioBuffer testBuffer = eden::AudioBuffer());
	}

	TEST(AudioBufferTest, Fill)
	{
		eden::AudioBuffer testBuffer(2, 1024);

		const std::vector<double> testValues { 0, 0.5, 0.6, 0.33, 0.1 };
		
		for (size_t k = 0; k < testValues.size(); k++)
		{
			testBuffer.fill(static_cast<eden::AudioBuffer::SampleType>(testValues[k]));

			for (auto i = 0; i < testBuffer.getNumChannels(); ++i)
			{
				for (auto j = 0u; j < testBuffer.getNumSamples(); ++j)
				{
					if (typeid(eden::AudioBuffer::SampleType) == typeid(float))
					{
						EXPECT_FLOAT_EQ(testBuffer.getArrayOfWritePointers()[i][j], static_cast<eden::AudioBuffer::SampleType>(testValues[k]));
					}
					else
					{
						EXPECT_DOUBLE_EQ(testBuffer.getArrayOfWritePointers()[i][j], testValues[k]);
					}
				}
			}
		}
	}

	TEST(AudioBufferTest, AddSample)
	{
		eden::AudioBuffer testBuffer(1, 1024);

		constexpr float FREQUENCY = 1000.f;
		constexpr float SAMPLE_RATE = 48000.f;

		for (auto i = 0u; i < testBuffer.getNumSamples(); ++i)
		{
			testBuffer.addSample(0, i, std::sin(2.f * FREQUENCY * eden::math_constants::PI * i / SAMPLE_RATE));
		}
	}

}