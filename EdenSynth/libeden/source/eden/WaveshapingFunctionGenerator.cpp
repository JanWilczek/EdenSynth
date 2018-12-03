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

	std::vector<float> WaveshapingFunctionGenerator::generateIdentity(unsigned length) const
	{
		return _impl->generateIdentity(length);
	}

	std::vector<float> WaveshapingFunctionGenerator::generateTransferFunction(std::function<float(float)> generator, unsigned length) const
	{
		return _impl->generateTransferFunction(generator, length);
	}

	std::vector<float> WaveshapingFunctionGenerator::generateChebyshevPolynomial(unsigned order, unsigned length) const
	{
		return _impl->generateChebyshevPolynomial(order, length);
	}
}
