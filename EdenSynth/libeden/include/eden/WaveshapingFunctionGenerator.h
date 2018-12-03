#pragma once
/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include <vector>
#include <memory>

namespace eden
{
	class WaveshapingFunctionGeneratorImpl;

	class WaveshapingFunctionGenerator
	{
	public:
		enum class TransferFunctions
		{
			Identity,
			HyperbolicTangent,
		};

		WaveshapingFunctionGenerator();

		std::vector<float> generateTransferFunction(TransferFunctions functionName, unsigned int length = 48000) const noexcept(false);

		std::vector<float> generateChebyshevPolynomial(unsigned int order, unsigned int length = 48000) const;

	private:
		std::unique_ptr<WaveshapingFunctionGeneratorImpl> _impl;
	};
}
