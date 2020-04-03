#include "pch.h"
#include "utility/dsp/Correlation.h"


namespace libeden_test
{
	using eden::utility::dsp::correlation;
	using eden::utility::dsp::CorrelationMode;

	TEST(CorrelationTest, DiracImpulses)
	{
		const std::vector<float> dirac1{ 0.f, 1.f, 0.f, 0.f };
		const std::vector<float> dirac2{ 0.f, 0.f, 1.f, 0.f };

		const auto result = correlation<CorrelationMode::Full>(dirac1, dirac2);

		ASSERT_EQ(result.size(), 7u);
		ASSERT_NEAR(result[2], 1.f / 7.f, 1e-3);
		for (auto i = 0u; i < result.size(); ++i)
		{
			if (i != 2u)
			{
				ASSERT_EQ(result[i], 0.f);
			}
		}
	}
}
