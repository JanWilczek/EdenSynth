#pragma once
///
/// \author Jan Wilczek
/// \date 15.11.2018
///
#include <map>
#include <JuceHeader.h>
#include "WaveTablePathProvider.h"
#include "eden/EdenSynthesiser.h"
#include "eden/Oscillator.h"
#include "parameters/ParameterRefs.h"

namespace eden::plugin {
class OscillatorContainer {
public:
  static constexpr auto invalidWaveTableIndex = -1;

  OscillatorContainer(eden::EdenSynthesiser& synthesiser,
                      WaveTablePathProvider pathProvider,
                      unsigned numOscillators);

  void updateOscillatorParameters(
      const eden::plugin::OscillatorParametersContainer& parameters);

  const WaveTablePathProvider& getPathProvider() const;

private:
  eden::EdenSynthesiser& _synthesiser;
  WaveTablePathProvider _pathProvider;
  std::map<std::string, std::unique_ptr<eden::Oscillator>> _oscillators;
  std::map<std::string, int> _waveTableIndices;
  std::map<std::string, eden::WaveformGenerator> _generatorNames;
  std::map<std::string, bool> _isRealTime;
};
}  // namespace eden::plugin
