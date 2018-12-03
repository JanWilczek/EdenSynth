/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include "eden/WaveshapingFunctionGenerator.h"
#include "eden_impl/WaveshapingFunctionGeneratorImpl.h"

namespace eden
{
	WaveshapingFunctionGenerator::WaveshapingFunctionGenerator()
		: _impl(std::make_unique<WaveshapingFunctionGeneratorImpl>())
	{
	}

	std::vector<float> WaveshapingFunctionGenerator::generateTransferFunction(TransferFunctions functionName, unsigned length)
	{
		return _impl->generateTransferFunction(functionName, length);
	}

	std::vector<float> WaveshapingFunctionGenerator::generateChebyshevPolynomial(unsigned order, unsigned length)
	{
		return _impl->generateChebyshevPolynomial(order, length);
	}
}
