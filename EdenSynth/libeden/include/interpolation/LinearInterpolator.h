#pragma once
/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include "interpolation/IInterpolator.h"

namespace eden::interpolation
{
	/// <summary>
	/// Interpolates linearly between two points taking into consideration the distance from each of them to the supplied index.
	/// </summary>
	class LinearInterpolator : public IInterpolator
	{
	public:
		~LinearInterpolator() override;

		float interpolate(const std::vector<float>& discreteValues, float index) override;
	};
}
