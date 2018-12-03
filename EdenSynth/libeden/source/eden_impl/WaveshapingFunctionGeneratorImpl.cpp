/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include "eden_impl/WaveshapingFunctionGeneratorImpl.h"
#include <cmath>

namespace eden
{
	std::vector<float> WaveshapingFunctionGeneratorImpl::generateTransferFunction(WaveshapingFunctionGenerator::TransferFunctions functionName, unsigned length) const noexcept(false)
	{
		switch(functionName)
		{
		case TransferFunctions::Identity:
			return generateIdentity(length);
		case TransferFunctions::HyperbolicTangent:
			return generateHyperbolicTangent(length);
		default:
			throw std::invalid_argument("Invalid transfer function name.");
		}
	}

	std::vector<float> WaveshapingFunctionGeneratorImpl::generateChebyshevPolynomial(unsigned order, const unsigned length) const
	{
		if (order == 0u)
		{
			return std::vector<float>(length, 1.f);
		}
		
		if (order == 1u)
		{
			return generateIdentity(length);
		}

		auto Tn_minus1 = generateChebyshevPolynomial(0u, length);
		auto Tn = generateChebyshevPolynomial(1u, length);

		auto n = 1u;
		while (n != order)
		{
			std::vector<float> Tn_plus1(length);

			// Tn+1 = 2 * x * Tn(x) - Tn-1(x)
			for (auto i = 0u; i < length; ++i)
			{
				auto x = static_cast<float>(2 * i) / (length - 1u) - 1.f;

				Tn_plus1[i] = 2.f * x * Tn[i] - Tn_minus1[i];
			}

			Tn_minus1 = std::move(Tn);
			Tn = std::move(Tn_plus1);
			n++;
		}

		return Tn;
	}

	std::vector<float> WaveshapingFunctionGeneratorImpl::generateIdentity(const unsigned length) const
	{
		std::vector<float> output(length);

		const float dx = 2.f / (length - 1u);
		output[0] = -1.f;
		for (auto i = 1u; i < length; ++i)
		{
			output[i] = output[i - 1u] + dx;
		}

		return output;
	}

	std::vector<float> WaveshapingFunctionGeneratorImpl::generateHyperbolicTangent(const unsigned length) const
	{
		std::vector<float> output(length);
		
		const auto lowerBoundary = -2.f;
		const auto upperBoundary = 2.f;

		const auto dx = (upperBoundary - lowerBoundary) / (length - 1);
		auto x = lowerBoundary;
		
		for (auto i = 0u; i < length; ++i)
		{
			output[i] = std::tanh(x);

			x += dx;
		}

		return output;
	}
}
