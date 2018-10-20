#pragma once
/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include <memory>
#include "eden/SampleType.h"
#include "synth/wavetable/WaveTable.h"
#include "interpolation/IInterpolator.h"

namespace eden::synth::wavetable
{
	class Waveform
	{
	public:
		SampleType operator()(double phase);

	private:
		WaveTable _waveTable;
		std::shared_ptr<interpolation::IInterpolator> _interpolator;
	};
}
