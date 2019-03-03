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
		if (_pathProvider.size() == 0)
		{
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Assets error", 
				"Assets folder not found. The assets folder should be in the same directory as the plugin. The synthesiser won't play.", "OK");
			return;
		}

		const std::string prefix = "oscillator";

		for (auto i = 1u; i <= numOscillators; ++i)
		{
			const auto name = prefix + std::to_string(i);
			_waveTableIndices[name] = _pathProvider.nameToIndex("Sine");
			_generatorNames[name] = eden::WaveformGenerator::SawtoothRampUp;
			_isRealTime[name] = false;
			_oscillators[name] = _synthesiser.createAndAddOscillator(_synthesiser.createWaveTableOscillatorSource(_pathProvider.getPath(_waveTableIndices[name])));
		}
	}

	void OscillatorContainer::addOscillatorParameters(AudioProcessorValueTreeState& pluginParameters)
	{
		for (auto& oscillator : _oscillators)
		{
			const auto parameterPrefix = "generator." + oscillator.first;
			const auto namePrefix = String(oscillator.first).toUpperCase();

			pluginParameters.createAndAddParameter(parameterPrefix + ".isRealTime", namePrefix + " is real time", String(), NormalisableRange<float>(0.f, 1.f, 1.f), 0.f, nullptr, nullptr);
			pluginParameters.createAndAddParameter(parameterPrefix + ".waveTable", namePrefix + " wave table", String(), NormalisableRange<float>(0.f, static_cast<float>(_pathProvider.size() - 1u), 1.0f), static_cast<float>(_waveTableIndices[oscillator.first]),
				[this](float index) { return String(_pathProvider.indexToName(static_cast<size_t>(index))); },
				[this](String name) { return static_cast<float>(_pathProvider.nameToIndex(name.toStdString())); });
			pluginParameters.createAndAddParameter(parameterPrefix + ".generatorName", namePrefix + " generator name", String(), NormalisableRange<float>(0.f, 3.f, 1.f), 0.f, nullptr, nullptr);
			pluginParameters.createAndAddParameter(parameterPrefix + ".octaveTransposition", namePrefix + " octave transposition", "oct.", NormalisableRange<float>(-3.0f, 3.0f, 1.0f), 0.f, nullptr, nullptr);
			pluginParameters.createAndAddParameter(parameterPrefix + ".semitoneTransposition", namePrefix + " semitone transposition", "semit.", NormalisableRange<float>(-6.0f, 6.0f, 1.0f), 0.f, nullptr, nullptr);
			pluginParameters.createAndAddParameter(parameterPrefix + ".centTransposition", namePrefix + " cent transposition", "ct.", NormalisableRange<float>(-50.0f, 50.0f, 1.0f), 0.f, nullptr, nullptr);
			pluginParameters.createAndAddParameter(parameterPrefix + ".volume", namePrefix + " volume", String(), NormalisableRange<float>(0.f, 1.0f, 0.0001f, 0.4f), 1.f, nullptr, nullptr);
			pluginParameters.createAndAddParameter(parameterPrefix + ".on", namePrefix + " on/off", String(), NormalisableRange<float>(0.f, 1.f, 1.f), 1.f, nullptr, nullptr);
		}
	}

	void OscillatorContainer::updateOscillatorParameters(const AudioProcessorValueTreeState& pluginParameters)
	{
		for (auto& oscillator : _oscillators)
		{
			const auto& oscillatorName = oscillator.first;

			const auto parameterPrefix = "generator." + oscillatorName;

			const auto isRealTime = static_cast<bool>(*pluginParameters.getRawParameterValue(parameterPrefix + ".isRealTime"));
			const auto waveTableIndex = static_cast<size_t>(*pluginParameters.getRawParameterValue(parameterPrefix + ".waveTable"));
			const auto generatorName = static_cast<eden::WaveformGenerator>(static_cast<int>(*pluginParameters.getRawParameterValue(parameterPrefix + ".generatorName")));

			// handle the source of the oscillator
			if (isRealTime)
			{
				if (!_isRealTime[oscillatorName] // oscillator changed from wavetable to real-time
					|| _generatorNames[oscillatorName] != generatorName) // generator's name changed
				{
					_isRealTime[oscillatorName] = true;
					_generatorNames[oscillatorName] = generatorName;

					oscillator.second->setSource(_synthesiser.createRealtimeOscillatorSource(_generatorNames[oscillatorName]));
				}
			}
			else
			{
				if (_isRealTime[oscillatorName] // oscillator changed from real-time to wavetable
					|| _waveTableIndices[oscillatorName] != waveTableIndex) // wavetable changed
				{
					_isRealTime[oscillatorName] = false;
					_waveTableIndices[oscillatorName] = waveTableIndex;

					oscillator.second->setSource(_synthesiser.createWaveTableOscillatorSource(_pathProvider.getPath(_pathProvider.indexToName(_waveTableIndices[oscillatorName]))));
				}
			}

			oscillator.second->setOctaveTransposition(static_cast<int>(*pluginParameters.getRawParameterValue(parameterPrefix + ".octaveTransposition")));
			oscillator.second->setSemitoneTransposition(static_cast<int>(*pluginParameters.getRawParameterValue(parameterPrefix + ".semitoneTransposition")));
			oscillator.second->setCentTransposition(static_cast<int>(*pluginParameters.getRawParameterValue(parameterPrefix + ".centTransposition")));
			oscillator.second->setVolume(*pluginParameters.getRawParameterValue(parameterPrefix + ".volume"));
			oscillator.second->setOn(static_cast<bool>(*pluginParameters.getRawParameterValue(parameterPrefix + ".on")));
		}
	}

	const WaveTablePathProvider& OscillatorContainer::getPathProvider() const
	{
		return _pathProvider;
	}
}
