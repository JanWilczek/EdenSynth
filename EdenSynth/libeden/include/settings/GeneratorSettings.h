#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>
#include "synth/wavetable/WaveTable.h"
#include "eden/OscillatorParameters.h"

namespace eden::synth::wavetable
{
	class SignalGenerator;
}

namespace eden::settings
{
	class GeneratorSettings
	{
	public:
		void registerSignalGenerator(std::shared_ptr<synth::wavetable::SignalGenerator> signalGenerator);
		
		OscillatorId getAvailableOscillatorId();
		void setWaveTable(synth::wavetable::WaveTable waveTable);

	private:
		std::vector<std::shared_ptr<synth::wavetable::SignalGenerator>> _signalGenerators;
		OscillatorId _firstAvailableOscillatorId;
	};
}
