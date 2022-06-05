#pragma once
#include <vector>


namespace eden::utility::dsp
{
	enum class CorrelationMode { Full };

	/// Calculates correlation for energy signals, i.e.:
	/// R_xy[kappa] = sum_over_all_k(x[k + kappa] * y[k])
	/// The output and modes (CorrelationMode) are analogous to the numpy's correlate function.
	template<CorrelationMode Mode = CorrelationMode::Full>
	std::vector<float> correlation(const std::vector<float>& x, const std::vector<float>& y);

	template<>
	std::vector<float> correlation<CorrelationMode::Full>(const std::vector<float>& x, const std::vector<float>& y)
	{
		std::vector<float> result(x.size() + y.size() - 1u, 0.f);
		const auto sizeMinusOneHalved = static_cast<long long>(result.size() - 1u) / 2;
		for (auto kappa = -sizeMinusOneHalved; kappa <= sizeMinusOneHalved; ++kappa)
		{
			float sum = 0.f;
			for (auto k = 0LL; (k + kappa) < static_cast<long long>(x.size()) && k < static_cast<long long>(y.size()); ++k)
			{
				if (k + kappa >= 0)
				{
					sum += x[k + kappa] * y[k];
				}
			}

			result[kappa + sizeMinusOneHalved] = sum;
		}
		
		return result;
	}
}
