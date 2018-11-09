#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>
#include "synth/wavetable/WaveTable.h"

namespace eden::synth::wavetable
{
	class SignalGenerator;
	//class WaveTable;
}

namespace eden::settings
{
	class GeneratorSettings
	{
	public:
		void registerSignalGenerator(std::shared_ptr<synth::wavetable::SignalGenerator> signalGenerator);

		void setWaveTable(synth::wavetable::WaveTable waveTable);

	private:
		std::vector<std::shared_ptr<synth::wavetable::SignalGenerator>> _signalGenerators;
	};
}
