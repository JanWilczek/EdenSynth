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
		Waveform(WaveTable waveTable, std::shared_ptr<interpolation::IInterpolator> interpolator);

		SampleType operator()(double phase) const;
		void setWaveTable(WaveTable waveTable);
		void setInterpolator(std::shared_ptr<interpolation::IInterpolator> interpolator);

	private:
		WaveTable _waveTable;
		std::shared_ptr<interpolation::IInterpolator> _interpolator;
	};
}
