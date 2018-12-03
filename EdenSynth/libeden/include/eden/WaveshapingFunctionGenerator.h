#pragma once
/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include <vector>
#include <memory>
#include <functional>

namespace eden
{
	class WaveshapingFunctionGeneratorImpl;

	class WaveshapingFunctionGenerator
	{
	public:
		WaveshapingFunctionGenerator();

		std::vector<float> generateIdentity(unsigned length) const;

		std::vector<float> generateTransferFunction(std::function<float(float)> generator, unsigned int length = 48000) const;

		std::vector<float> generateChebyshevPolynomial(unsigned int order, unsigned int length = 48000) const;

	private:
		std::unique_ptr<WaveshapingFunctionGeneratorImpl> _impl;
	};
}
