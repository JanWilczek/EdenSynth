#pragma once
/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include "interpolation/IInterpolator.h"

namespace eden::interpolation
{
	class LinearInterpolator : public IInterpolator
	{
	public:
		~LinearInterpolator() override;

		SampleType interpolate(const std::vector<SampleType>& discreteValues, double index) override;
	};
}