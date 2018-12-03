/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include "eden/WaveshapingFunctionGenerator.h"
#include "eden_impl/WaveshapingFunctionGeneratorImpl.h"

namespace eden
{
	std::vector<float> WaveshapingFunctionGenerator::generateIdentity(unsigned length)
	{
		return WaveshapingFunctionGeneratorImpl::generateIdentity(length);
	}

	std::vector<float> WaveshapingFunctionGenerator::generateTransferFunction(std::function<float(float)> generator, unsigned length)
	{
		return WaveshapingFunctionGeneratorImpl::generateTransferFunction(generator, length);
	}

	std::vector<float> WaveshapingFunctionGenerator::generateChebyshevPolynomial(unsigned order, unsigned length)
	{
		return WaveshapingFunctionGeneratorImpl::generateChebyshevPolynomial(order, length);
	}
}
