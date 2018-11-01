/// 
/// \author Jan Wilczek
/// \date 31.10.2018
/// 
#include "pch.h"
#include <interpolation/LinearInterpolator.h>

namespace libeden_test
{
	TEST(LinearInterpolatorTest, Interpolate)
	{
		std::unique_ptr<eden::interpolation::IInterpolator> interpolator = std::make_unique<eden::interpolation::LinearInterpolator>();

		const std::vector<eden::SampleType> signal = { 0.f, 1.f, 2.f, 3.f, 5.f, 6.f };
		
		const auto result1 = interpolator->interpolate(signal, 0.5);
		EXPECT_FLOAT_EQ(result1, 0.5f);

		const auto result2 = interpolator->interpolate(signal, 2);
		EXPECT_FLOAT_EQ(result2, 2.f);

		const auto result3 = interpolator->interpolate(signal, 3.5f);
		EXPECT_FLOAT_EQ(result3, 4.f);

		const auto result4 = interpolator->interpolate(signal, 5.f);
		EXPECT_FLOAT_EQ(result4, 6.f);

		EXPECT_DEATH(interpolator->interpolate(signal, 5.1f), "");
	}
}
