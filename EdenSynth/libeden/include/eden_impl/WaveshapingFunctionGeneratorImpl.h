#pragma once
/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include <vector>
#include <functional>

namespace eden
{
	class WaveshapingFunctionGeneratorImpl
	{
	public:
		static std::vector<float> generateIdentity(unsigned length);

		static std::vector<float> generateTransferFunction(std::function<float(float)> generator, unsigned int length);

		static std::vector<float> generateChebyshevPolynomial(unsigned int order, unsigned int length);

		static void spreadValuesRandomly(std::vector<float>& function, float spreadRange);
	};
}
