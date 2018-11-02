#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.11.2018
/// 
#include "pch.h"
#include <eden/SampleType.h>

namespace libeden_test
{
	class TestUtils
	{
	public:
		static void fillChannel(eden::SampleType* channel, eden::SampleType value, int begin, int end)
		{
			for (auto i = begin; i < end; ++i)
			{
				channel[i] = value;
			}
		}

		static double calculateFrequency(const eden::SampleType* channel, int bufferSize, double sampleRate)
		{
			unsigned zeroCrossingsCount = 0u;
			int startIndex, stopIndex;

			int numberOfPeriods = 0, periodsLength = 0;

			for (auto i = 1; i < bufferSize; ++i)
			{
				if (channel[i] * channel[i-1] < 0.f)
				{
					++zeroCrossingsCount;

					if (zeroCrossingsCount == 1)
					{
						startIndex = i;
					}
					else if (zeroCrossingsCount == 3)
					{
						zeroCrossingsCount = 1;
						stopIndex = i;

						++numberOfPeriods;
						periodsLength += stopIndex - startIndex;

						startIndex = stopIndex;
					}
				}
			}

			if (numberOfPeriods < 1)
			{
				return 0.0;
			}

			const auto meanPeriod = (static_cast<double>(periodsLength) / numberOfPeriods) / sampleRate;
			return 1 / meanPeriod;
		}
	};
}
