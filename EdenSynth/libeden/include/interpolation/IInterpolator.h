#pragma once
/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include <vector>
#include "eden/SampleType.h"

namespace eden::interpolation
{
	class IInterpolator
	{
	public:
		virtual ~IInterpolator() = 0;

		virtual SampleType interpolate(const std::vector<SampleType>& discreteValues, double index) = 0;
	};
}
