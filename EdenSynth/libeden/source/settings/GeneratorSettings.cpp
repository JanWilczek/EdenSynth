/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/GeneratorSettings.h"
#include "synth/wavetable/SignalGenerator.h"
#include "synth/wavetable/WaveTableSource.h"
#include "synth/wavetable/va_sources/SawtoothVASource.h"
#include "synth/wavetable/va_sources/PulseVASource.h"

namespace eden::settings
{
	void GeneratorSettings::registerSignalGenerator(std::shared_ptr<synth::wavetable::SignalGenerator> signalGenerator)
	{
		_signalGenerators.push_back(signalGenerator);
	}

	void GeneratorSettings::setSampleRate(float sampleRate)
	{
		for (auto generator : _signalGenerators)
		{
			generator->setSampleRate(sampleRate);
		}
	}

	OscillatorSourceId GeneratorSettings::createGeneratorSource(float sampleRate, WaveformGenerator generatorName)
	{
		std::unique_ptr<synth::wavetable::IOscillatorSource> source;

		switch (generatorName)
		{
		case WaveformGenerator::SawtoothRampUp:
			source = std::make_unique<synth::wavetable::SawtoothVASource>(sampleRate);
			break;
		case WaveformGenerator::Square:
			source = std::make_unique<synth::wavetable::PulseVASource>(sampleRate);
			break;
		default:
			throw std::logic_error("Generator not implemented.");
		}

		const auto id = _firstAvailableSourceId++;

		_oscillatorSources[id] = std::move(source);

		return id;
	}

	OscillatorSourceId GeneratorSettings::createWaveTableSource(float sampleRate, std::vector<float> waveTable)
	{
		const auto id = _firstAvailableSourceId++;

		auto source = std::make_unique<synth::wavetable::WaveTableSource>(sampleRate);
		source->setWaveTable(waveTable);

		_oscillatorSources[id] = std::move(source);

		return id;
	}

	void GeneratorSettings::removeOscillatorSource(OscillatorSourceId sourceId)
	{
		const auto erased = _oscillatorSources.erase(sourceId);

		if (erased != 1u)
		{
			throw std::runtime_error("Invalid oscillator source ID.");
		}
	}

	OscillatorId GeneratorSettings::addOscillator(OscillatorSourceId sourceId)
	{
		if (_oscillatorSources.find(sourceId) == _oscillatorSources.end())
		{
			throw std::runtime_error("Invalid oscillator source ID.");
		}

		const auto id = _firstAvailableOscillatorId++;

		for (auto generator : _signalGenerators)
		{
			generator->addOscillator(synth::wavetable::SynthOscillator(id, _oscillatorSources[id]->clone()));
		}

		return id;
	}

	void GeneratorSettings::removeOscillator(OscillatorId oscillatorToRemove)
	{
		for (auto generator : _signalGenerators)
		{
			generator->removeOscillator(oscillatorToRemove);
		}
	}

	void GeneratorSettings::setOscillatorSource(OscillatorId oscillatorId, OscillatorSourceId sourceId)
	{
		if (_oscillatorSources.find(sourceId) == _oscillatorSources.end())
		{
			throw std::runtime_error("Invalid oscillator source ID.");
		}

		for (auto generator : _signalGenerators)
		{
			generator->setOscillatorSource(oscillatorId, _oscillatorSources[sourceId]->clone());
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

	void GeneratorSettings::setOscillatorVolume(OscillatorId oscillatorId, float volume)
	{
		for (auto generator : _signalGenerators)
		{
			generator->setOscillatorVolume(oscillatorId, volume);
		}
	}

	void GeneratorSettings::setOscillatorOn(OscillatorId oscillatorId, bool isOn)
	{
		for (auto generator : _signalGenerators)
		{
			generator->setOscillatorOn(oscillatorId, isOn);
		}
	}
}
