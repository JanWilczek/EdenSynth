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
		using TransferFunctions = WaveshapingFunctionGenerator::TransferFunctions;

		std::vector<float> generateTransferFunction(WaveshapingFunctionGenerator::TransferFunctions functionName, unsigned int length) const noexcept(false);

		std::vector<float> generateChebyshevPolynomial(unsigned int order, unsigned int length) const;

	private:
		std::vector<float> generateIdentity(unsigned length) const;

		std::vector<float> generateHyperbolicTangent(unsigned length) const;

	};
}
