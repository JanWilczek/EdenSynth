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
	OscillatorId GeneratorSettings::getAvailableOscillatorId()
	{
		return _firstAvailableOscillatorId++;
	}
	void GeneratorSettings::addOscillator(synth::wavetable::SynthOscillator oscillator)
	{
		for (auto generator : _signalGenerators)
		{
			generator->addOscillator(oscillator);
		}
	}

	void GeneratorSettings::removeOscillator(OscillatorId oscillatorToRemove)
	{
		for (auto generator : _signalGenerators)
		{
			generator->removeOscillator(oscillatorToRemove);
		}
	}

	void GeneratorSettings::setOscillatorSource(OscillatorId oscillatorId, std::unique_ptr<synth::wavetable::IOscillatorSource> source)
	{
		for (auto generator : _signalGenerators)
		{
			generator->setOscillatorSource(oscillatorId, source->clone());
		}
	}

	void GeneratorSettings::setOctaveTransposition(OscillatorId oscillatorId, int octaveShift)
	{
		for (auto generator : _signalGenerators)
		{
			generator->setOctaveTransposition(oscillatorId, octaveShift);
		}
	}

	void GeneratorSettings::setSemitoneTransposition(OscillatorId oscillatorId, int semitoneShift)
	{
		for (auto generator : _signalGenerators)
		{
			generator->setSemitoneTransposition(oscillatorId, semitoneShift);
		}
	}

	void GeneratorSettings::setCentTransposition(OscillatorId oscillatorId, int centShift)
	{
		for (auto generator : _signalGenerators)
		{
			generator->setCentTransposition(oscillatorId, centShift);
		}
	}
}
