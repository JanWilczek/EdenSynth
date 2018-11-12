/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include "synth/wavetable/Waveform.h"
#include "utility/MathConstants.h"
#include "utility/EdenAssert.h"

namespace eden::synth::wavetable
{
	Waveform::Waveform(WaveTable waveTable, std::shared_ptr<interpolation::IInterpolator> interpolator)
		: _waveTable(waveTable)
		, _interpolator(interpolator)
	{
	}

	SampleType Waveform::operator()(float phase) const
	{
		const int num2PI = static_cast<int>(phase / (2 * math_constants::PI));
		phase -= num2PI * 2 * math_constants::PI;

		EDEN_ASSERT(phase >= 0 && phase < 2 * math_constants::PI);

		const double index = (phase / (2 * math_constants::PI)) * _waveTable.size();

		return _interpolator->interpolate(_waveTable, index);
	}

	void Waveform::setWaveTable(WaveTable waveTable)
	{
		_waveTable = waveTable;
	}

	void Waveform::setInterpolator(std::shared_ptr<interpolation::IInterpolator> interpolator)
	{
		_interpolator = interpolator;
	}
}
