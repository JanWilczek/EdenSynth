/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include "interpolation/LinearInterpolator.h"
#include "utility/EdenAssert.h"
#include <cmath>

namespace eden::interpolation
{
	LinearInterpolator::~LinearInterpolator()
	{
	}

	SampleType LinearInterpolator::interpolate(const std::vector<SampleType>& discreteValues, double index)
	{
		EDEN_ASSERT(index >= 0 && index < discreteValues.size());

		const auto lowerIndex = static_cast<int>(std::floor(index));
		const auto upperIndex = static_cast<int>(std::ceil(index));

		const auto toLower = index - lowerIndex;
		const auto toUpper = upperIndex - index;

		return toLower * discreteValues[upperIndex % discreteValues.size()] + toUpper * discreteValues[lowerIndex];
	}
}
