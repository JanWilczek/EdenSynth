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
		std::vector<float> generateTransferFunction(WaveshapingFunctionGenerator::TransferFunctions functionName, unsigned int length);

		std::vector<float> generateChebyshevPolynomial(unsigned int order, unsigned int length);
	};
}
