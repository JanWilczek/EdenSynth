/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include "synth/waveshaping/Waveshaper.h"
#include <algorithm>
#include "interpolation/LinearInterpolator.h"

namespace eden::synth::waveshaping
{
	Waveshaper::Waveshaper()
		: _makeUpGainEnabled(false)
		, _makeUpGainFactor(1.f)
		, _interpolator(std::make_unique<interpolation::LinearInterpolator>())
	{
	}

	float Waveshaper::transform(float sample)
	{
		const auto index = sample * static_cast<float>(static_cast<int>(_transferFunction.size()) / 2);

		auto output = _interpolator->interpolate(_transferFunction, index);

		if (_makeUpGainEnabled)
		{
			output *= _makeUpGainFactor;
		}

		return output;
	}

	void Waveshaper::setTransferFunction(std::vector<float> transferFunction)
	{
		_transferFunction = std::move(transferFunction);

		_makeUpGainFactor = 1.f / std::min(*std::max_element(_transferFunction.begin(), _transferFunction.end()), *std::min_element(_transferFunction.begin(), _transferFunction.end()));
	}

	void Waveshaper::setAutoMakeUpGain(bool makeUpGainEnabled)
	{
		_makeUpGainEnabled = makeUpGainEnabled;
	}
}
