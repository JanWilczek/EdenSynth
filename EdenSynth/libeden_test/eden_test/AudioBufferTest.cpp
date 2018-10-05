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
			for (int j = 0; j < testBuffer.getNumSamples(); ++j)
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
}