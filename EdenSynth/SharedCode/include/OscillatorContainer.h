#pragma once
///
/// \author Jan Wilczek
/// \date 15.11.2018
///
#include <map>
#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveTablePathProvider.h"
#include "eden/EdenSynthesiser.h"
#include "eden/Oscillator.h"

namespace eden_vst {
class OscillatorContainer {
public:
  OscillatorContainer(eden::EdenSynthesiser& synthesiser,
                      WaveTablePathProvider pathProvider,
                      unsigned numOscillators);

  void addOscillatorParameters(AudioProcessorValueTreeState& pluginParameters);
  void updateOscillatorParameters(
      const AudioProcessorValueTreeState& pluginParameters);
  const WaveTablePathProvider& getPathProvider() const;

private:
  eden::EdenSynthesiser& _synthesiser;
  WaveTablePathProvider _pathProvider;
  std::map<std::string, std::unique_ptr<eden::Oscillator>> _oscillators;
  std::map<std::string, size_t> _waveTableIndices;
  std::map<std::string, eden::WaveformGenerator> _generatorNames;
  std::map<std::string, bool> _isRealTime;
};
}  // namespace eden_vst
