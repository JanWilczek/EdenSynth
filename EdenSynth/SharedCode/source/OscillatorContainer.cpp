///
/// \author Jan Wilczek
/// \date 15.11.2018
///
#include "OscillatorContainer.h"

#include "synth/wavetable/SineWaveTable.h"
#include <ranges>

namespace eden::plugin {
OscillatorContainer::OscillatorContainer(eden::EdenSynthesiser& synthesiser,
                                         WaveTablePathProvider pathProvider,
                                         unsigned numOscillators)
    : _synthesiser(synthesiser), _pathProvider(std::move(pathProvider)) {
  if (_pathProvider.size() == 0) {
    DBG("Assets folder not found. The assets folder should be in the same "
        << "directory as the plugin. The synthesiser won't play.");
  }

  const std::string prefix = "osc";

  for (auto i = 1u; i <= numOscillators; ++i) {
    const auto name = prefix + std::to_string(i);
    _waveTableIndices[name] = invalidWaveTableIndex;
    _generatorNames[name] = eden::WaveformGenerator::Sine;
    _isRealTime[name] = true;
    _oscillators[name] = _synthesiser.createAndAddOscillator(
        _synthesiser.createRealtimeOscillatorSource(_generatorNames[name]));
  }
}

void OscillatorContainer::updateOscillatorParameters(
    const eden::plugin::OscillatorParametersContainer& parameters) {
  for (const auto& [oscillator, params] :
       std::views::zip(_oscillators, parameters)) {
    const auto& oscillatorName = oscillator.first;

    const auto isRealTime = static_cast<bool>(params.isRealTime.get());

    const auto waveTableIndex = static_cast<int>(params.waveTable.get());
    const auto generatorName = static_cast<eden::WaveformGenerator>(
        static_cast<int>(params.generatorName.get()));

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

        if (_pathProvider.size() > 0u &&
            waveTableIndex != invalidWaveTableIndex) {
          oscillator.second->setSource(
              _synthesiser.createWaveTableOscillatorSource(
                  _pathProvider.getPath(
                      _pathProvider.indexToName(static_cast<size_t>(
                          _waveTableIndices[oscillatorName])))));
        } else {
          oscillator.second->setSource(
              _synthesiser.createWaveTableOscillatorSource(
                  eden::synth::wavetable::SineWaveTable));
        }
      }
    }

    oscillator.second->setOctaveTransposition(
        static_cast<int>(params.octaveTransposition));
    oscillator.second->setSemitoneTransposition(
        static_cast<int>(params.semitoneTransposition));
    oscillator.second->setCentTransposition(
        static_cast<int>(params.centTransposition));
    oscillator.second->setVolume(params.volume);
    oscillator.second->setOn(static_cast<bool>(params.on));
  }
}

const WaveTablePathProvider& OscillatorContainer::getPathProvider() const {
  return _pathProvider;
}
}  // namespace eden::plugin
