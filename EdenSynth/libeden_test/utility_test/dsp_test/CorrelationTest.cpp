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
	}
}
