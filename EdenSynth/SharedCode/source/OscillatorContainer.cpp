/// 
/// \author Jan Wilczek
/// \date 15.11.2018
/// 
#include "OscillatorContainer.h"

namespace eden_vst
{
	OscillatorContainer::OscillatorContainer(eden::EdenSynthesiser& synthesiser, WaveTablePathProvider pathProvider, unsigned numOscillators)
		: _synthesiser(synthesiser)
		, _pathProvider(std::move(pathProvider))
	{
		const std::string prefix = "oscillator";

		for (auto i = 1u; i <= numOscillators; ++i)
		{
			const auto name = prefix + std::to_string(i);
			_waveTableIndices[name] = _pathProvider.nameToIndex("Sine");
			_oscillators[name] = _synthesiser.createAndAddOscillator(_synthesiser.createWaveTableOscillatorSource(_pathProvider.getPath(_waveTableIndices[name])));
		}
	}

	void OscillatorContainer::addOscillatorParameters(AudioProcessorValueTreeState& pluginParameters)
	{
		for (auto& oscillator : _oscillators)
		{
			pluginParameters.createAndAddParameter("generator." + oscillator.first + ".waveTable", "Wave table", String(), NormalisableRange<float>(0.f, static_cast<float>(_pathProvider.size() - 1u), 1.0f), static_cast<float>(_waveTableIndices[oscillator.first]),
				[this](float index) { return String(_pathProvider.indexToName(static_cast<size_t>(index))); },
				[this](String name) { return static_cast<float>(_pathProvider.nameToIndex(name.toStdString())); });
			pluginParameters.createAndAddParameter("generator." + oscillator.first + ".octaveTransposition", "Octave transposition", "oct", NormalisableRange<float>(-3.0f, 3.0f, 1.0f), 0.f, nullptr, nullptr);
			pluginParameters.createAndAddParameter("generator." + oscillator.first + ".semitoneTransposition", "Semitone transposition", "semit.", NormalisableRange<float>(-6.0f, 6.0f, 1.0f), 0.f, nullptr, nullptr);
			pluginParameters.createAndAddParameter("generator." + oscillator.first + ".centTransposition", "Cent transposition", "ct.", NormalisableRange<float>(-50.0f, 50.0f, 1.0f), 0.f, nullptr, nullptr);
			pluginParameters.createAndAddParameter("generator." + oscillator.first + ".volume", "Volume", String(), NormalisableRange<float>(0.f, 1.0f, 0.0001f, 0.4f), 1.f, nullptr, nullptr);
			pluginParameters.createAndAddParameter("generator." + oscillator.first + ".on", "On/Off", String(), NormalisableRange<float>(0.f, 1.f, 1.f), 1.f, nullptr, nullptr);
		}
	}

	void OscillatorContainer::updateOscillatorParameters(const AudioProcessorValueTreeState& pluginParameters)
	{
		for (auto& oscillator : _oscillators)
		{
			const auto waveTableIndex = static_cast<size_t>(*pluginParameters.getRawParameterValue("generator." + oscillator.first + ".waveTable"));
			if (waveTableIndex != _waveTableIndices[oscillator.first])
			{
				_waveTableIndices[oscillator.first] = waveTableIndex;
				oscillator.second->setSource(_synthesiser.createWaveTableOscillatorSource(_pathProvider.getPath(_pathProvider.indexToName(_waveTableIndices[oscillator.first]))));
			}
			oscillator.second->setOctaveTransposition(*pluginParameters.getRawParameterValue("generator." + oscillator.first + ".octaveTransposition"));
			oscillator.second->setSemitoneTransposition(*pluginParameters.getRawParameterValue("generator." + oscillator.first + ".semitoneTransposition"));
			oscillator.second->setCentTransposition(*pluginParameters.getRawParameterValue("generator." + oscillator.first + ".centTransposition"));
			oscillator.second->setVolume(*pluginParameters.getRawParameterValue("generator." + oscillator.first + ".volume"));
			oscillator.second->setOn(static_cast<bool>(*pluginParameters.getRawParameterValue("generator." + oscillator.first + ".on")));
		}
	}

	WaveTablePathProvider& OscillatorContainer::getPathProvider()
	{
		return _pathProvider;
	}
}
