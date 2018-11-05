#pragma once
/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include <vector>
#include <cmath>
#include "eden/SampleType.h"
#include "utility/MathConstants.h"

namespace eden::synth::wavetable
{
	const std::vector<SampleType> SineWaveTable = []()
	{
		constexpr size_t size = 48000u;
		std::vector<SampleType> result(size);
		for (size_t i = 0; i < size; ++i)
		{
			result[i] = static_cast<SampleType>(std::sin(2 * math_constants::PI * i / size));
		}
		
		return result;
	}();
}
