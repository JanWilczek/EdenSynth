#pragma once
#include <vector>


namespace eden::utility::dsp
{
	enum class CorrelationMode { Full };
	
	template<CorrelationMode Mode = CorrelationMode::Full>
	std::vector<float> correlation(const std::vector<float>& x, const std::vector<float>& y);

	template<>
	std::vector<float> correlation<CorrelationMode::Full>(const std::vector<float>& x, const std::vector<float>& y)
	{
		return std::vector<float>{};
	}
}
