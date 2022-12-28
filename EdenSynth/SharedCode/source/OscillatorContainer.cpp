///
/// \author Jan Wilczek
/// \date 15.11.2018
///
#include "OscillatorContainer.h"

namespace eden_vst {
OscillatorContainer::OscillatorContainer(eden::EdenSynthesiser& synthesiser,
                                         WaveTablePathProvider pathProvider,
                                         unsigned numOscillators)
    : _synthesiser(synthesiser), _pathProvider(std::move(pathProvider)) {
  if (_pathProvider.size() == 0) {
    AlertWindow::showMessageBoxAsync(
        AlertWindow::AlertIconType::WarningIcon, "Assets error",
        "Assets folder not found. The assets folder should be in the same "
        "directory as the plugin. The synthesiser won't play.",
        "OK");
    return;
  }

  const std::string prefix = "oscillator";

  for (auto i = 1u; i <= numOscillators; ++i) {
    const auto name = prefix + std::to_string(i);
    _waveTableIndices[name] = _pathProvider.nameToIndex("Sine");
    _generatorNames[name] = eden::WaveformGenerator::SawtoothRampUp;
    _isRealTime[name] = false;
    _oscillators[name] = _synthesiser.createAndAddOscillator(
        _synthesiser.createWaveTableOscillatorSource(
            _pathProvider.getPath(_waveTableIndices[name])));
  }
}

void OscillatorContainer::addOscillatorParameters(
    AudioProcessorValueTreeState& pluginParameters) {
  using Parameter = juce::AudioProcessorValueTreeState::Parameter;

  for (auto& oscillator : _oscillators) {
    const auto parameterPrefix = "generator." + oscillator.first;
    const auto namePrefix = String(oscillator.first).toUpperCase();

    pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
        parameterPrefix + ".isRealTime", namePrefix + " is real time",
        NormalisableRange<float>(0.f, 1.f, 1.f), 0.f));
    pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
        parameterPrefix + ".waveTable", namePrefix + " wave table",
        NormalisableRange<float>(
            0.f, static_cast<float>(_pathProvider.size() - 1u), 1.0f),
        static_cast<float>(_waveTableIndices[oscillator.first]),
        AudioProcessorValueTreeStateParameterAttributes{}
            .withStringFromValueFunction(
        [this](float index, int maximumLength) {
          return String(_pathProvider.indexToName(static_cast<size_t>(index))).substring(0, maximumLength);
            })
            .withValueFromStringFunction(
        [this](String name) {
          return static_cast<float>(
              _pathProvider.nameToIndex(name.toStdString()));
        })));
    pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
        parameterPrefix + ".generatorName", namePrefix + " generator name",
        NormalisableRange<float>(0.f, 4.f, 1.f), 0.f));
    pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
        parameterPrefix + ".octaveTransposition",
        namePrefix + " octave transposition",
        NormalisableRange<float>(-3.0f, 3.0f, 1.0f), 0.f,
        AudioProcessorValueTreeStateParameterAttributes{}.withLabel("oct.")));
    pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
        parameterPrefix + ".semitoneTransposition",
        namePrefix + " semitone transposition",
        NormalisableRange<float>(-6.0f, 6.0f, 1.0f), 0.f,
        AudioProcessorValueTreeStateParameterAttributes{}.withLabel("semit.")));
    pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
        parameterPrefix + ".centTransposition",
        namePrefix + " cent transposition",
        NormalisableRange<float>(-50.0f, 50.0f, 1.0f), 0.f,
        AudioProcessorValueTreeStateParameterAttributes{}.withLabel("ct.")));
    pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
        parameterPrefix + ".volume", namePrefix + " volume",
        NormalisableRange<float>(0.f, 1.0f, 0.0001f, 0.4f), 1.f));
    pluginParameters.createAndAddParameter(std::make_unique<Parameter>(
        parameterPrefix + ".on", namePrefix + " on/off",
        NormalisableRange<float>(0.f, 1.f, 1.f), 1.f));
  }
}

void OscillatorContainer::updateOscillatorParameters(
    const AudioProcessorValueTreeState& pluginParameters) {
  for (auto& oscillator : _oscillators) {
    const auto& oscillatorName = oscillator.first;

    const auto parameterPrefix = "generator." + oscillatorName;

    const auto isRealTime =
        static_cast<bool>(*pluginParameters.getRawParameterValue(
            parameterPrefix + ".isRealTime"));
    const auto waveTableIndex = static_cast<size_t>(
        *pluginParameters.getRawParameterValue(parameterPrefix + ".waveTable"));
    const auto generatorName = static_cast<eden::WaveformGenerator>(
        static_cast<int>(*pluginParameters.getRawParameterValue(
            parameterPrefix + ".generatorName")));

    // handle the source of the oscillator
    if (isRealTime) {
      if (!_isRealTime[oscillatorName]  // oscillator changed from wavetable to
                                        // real-time
          || _generatorNames[oscillatorName] !=
                 generatorName)  // generator's name changed
      {
        _isRealTime[oscillatorName] = true;
        _generatorNames[oscillatorName] = generatorName;

        oscillator.second->setSource(
            _synthesiser.createRealtimeOscillatorSource(
                _generatorNames[oscillatorName]));
      }
    } else {
      if (_isRealTime[oscillatorName]  // oscillator changed from real-time to
                                       // wavetable
          || _waveTableIndices[oscillatorName] !=
                 waveTableIndex)  // wavetable changed
      {
        _isRealTime[oscillatorName] = false;
        _waveTableIndices[oscillatorName] = waveTableIndex;

        oscillator.second->setSource(
            _synthesiser.createWaveTableOscillatorSource(_pathProvider.getPath(
                _pathProvider.indexToName(_waveTableIndices[oscillatorName]))));
      }
    }

    oscillator.second->setOctaveTransposition(
        static_cast<int>(*pluginParameters.getRawParameterValue(
            parameterPrefix + ".octaveTransposition")));
    oscillator.second->setSemitoneTransposition(
        static_cast<int>(*pluginParameters.getRawParameterValue(
            parameterPrefix + ".semitoneTransposition")));
    oscillator.second->setCentTransposition(
        static_cast<int>(*pluginParameters.getRawParameterValue(
            parameterPrefix + ".centTransposition")));
    oscillator.second->setVolume(
        *pluginParameters.getRawParameterValue(parameterPrefix + ".volume"));
    oscillator.second->setOn(static_cast<bool>(
        *pluginParameters.getRawParameterValue(parameterPrefix + ".on")));
  }
}

const WaveTablePathProvider& OscillatorContainer::getPathProvider() const {
  return _pathProvider;
}
}  // namespace eden_vst
