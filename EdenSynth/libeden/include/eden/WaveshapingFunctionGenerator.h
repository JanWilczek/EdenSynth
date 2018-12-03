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
			LightDistortion,
			HeavyDistortion,
			HyperbolicTangent,
		};

		WaveshapingFunctionGenerator();

		std::vector<float> generateTransferFunction(TransferFunctions functionName, unsigned int length = 48000);

		std::vector<float> generateChebyshevPolynomial(unsigned int order, unsigned int length = 48000);

	private:

		std::unique_ptr<WaveshapingFunctionGeneratorImpl> _impl;
	};
}
