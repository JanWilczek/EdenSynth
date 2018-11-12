/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/GeneratorSettings.h"
#include "synth/wavetable/SignalGenerator.h"

namespace eden::settings
{
	void GeneratorSettings::registerSignalGenerator(std::shared_ptr<synth::wavetable::SignalGenerator> signalGenerator)
	{
		_signalGenerators.push_back(signalGenerator);
	}

	/*void GeneratorSettings::setWaveTable(synth::wavetable::WaveTable waveTable)
	{
		for (auto generator : _signalGenerators)
		{
			generator->setWaveTable(waveTable);
		}
	}*/
}
