#pragma once
/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include <vector>
#include "eden/WaveshapingFunctionGenerator.h"

namespace eden
{
	class WaveshapingFunctionGeneratorImpl
	{
	public:
		std::vector<float> generateIdentity(unsigned length) const;

		std::vector<float> generateTransferFunction(std::function<float(float)> generator, unsigned int length) const;

		std::vector<float> generateChebyshevPolynomial(unsigned int order, unsigned int length) const;
	};
}
