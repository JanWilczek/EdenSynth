#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include <memory>
#include <vector>
#include "synth/wavetable/IOscillatorSource.h"
#include "eden/OscillatorParameters.h"

namespace eden
{
	class OscillatorSourceImpl
	{
	public:
		static std::unique_ptr<OscillatorSourceImpl> createRealtimeOscillatorSource(WaveformGenerators generatorName);

		static std::unique_ptr<OscillatorSourceImpl> createWaveTableOscillatorSource(std::vector<SampleType> waveTable);

	private:
		OscillatorSourceImpl(std::unique_ptr<synth::wavetable::IOscillatorSource> synthOscillatorSource);

		std::unique_ptr<synth::wavetable::IOscillatorSource> _synthOscillatorSource;
	};
}
