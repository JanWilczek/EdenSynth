#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.11.2018
/// 
#include "pch.h"

#include "fftw3.h"
#include <complex>

using Complex = std::complex<double>;


namespace libeden_test
{
	class TestUtils
	{
	public:
		/// <summary>
		/// Fills audio <paramref name="channel"/> with given <paramref name="value"/> from <paramref name="begin"/>
		/// (including) to <paramref name="end"/> (excluding).
		/// </summary>
		/// <param name="channel"></param>
		/// <param name="value"></param>
		/// <param name="begin"></param>
		/// <param name="end"></param>
		static void fillChannel(float* channel, float value, int begin, int end)
		{
			for (auto i = begin; i < end; ++i)
			{
				channel[i] = value;
			}
		}

		/// <summary>
		/// Calculates frequency of a signal in <paramref name="channel"/> using zero crossings method.
		/// </summary>
		/// <param name="channel"></param>
		/// <param name="bufferSize"></param>
		/// <param name="sampleRate"></param>
		/// <returns>detected frequency in Hz, 0 if no period has been found</returns>
		static double detectFrequency(const float* channel, int bufferSize, double sampleRate)
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

		static std::vector<Complex> dft(const std::vector<float>& real1dSignal)
		{
			const auto transformationSize = static_cast<int>(real1dSignal.size());

			double* inBuffer = new double[real1dSignal.size()];
			std::copy(real1dSignal.begin(), real1dSignal.end(), inBuffer);

			fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * transformationSize);

			fftw_plan plan = fftw_plan_dft_r2c_1d(transformationSize, inBuffer, out, FFTW_ESTIMATE);

			fftw_execute(plan);

			std::vector<Complex> result(transformationSize);
			for (int i = 0u; i < transformationSize; ++i)
			{
				result[i] = Complex{ out[i][0], out[i][1] };
			}

			fftw_destroy_plan(plan);
			fftw_free(out);
			delete[] inBuffer;

			return result;;
		}

		static std::vector<double> magnitude(const std::vector<Complex>& dft)
		{
			std::vector<double> magnitude;

			for (const auto coefficient : dft)
			{
				magnitude.push_back(std::sqrt(std::pow(coefficient.real(), 2) + std::pow(coefficient.imag(), 2)));
			}

			return magnitude;
		}

	};
}
