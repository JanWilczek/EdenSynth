#pragma once
///
/// \author Jan Wilczek
/// \date 02.09.2018
///
#include <filesystem>
#include <JuceHeader.h>
#include "EdenFilterParameters.h"
#include "EdenWaveshapingParameters.h"
#include "OscillatorContainer.h"
#include "WaveTablePathProvider.h"
#include <wolfsound/juce/wolfsound_ParameterHolder.hpp>

namespace eden {
class EdenSynthesiser;
class MidiBuffer;
}  // namespace eden

namespace eden::plugin {
/// <summary>
/// Class containing utilities connecting JUCE with Eden.
/// </summary>
class EdenAdapter {
public:
  EdenAdapter(eden::EdenSynthesiser& synthesiser,
              juce::AudioProcessorValueTreeState&,
              wolfsound::JuceParameterHolder::Builder&,
              std::filesystem::path assetsPath);

  /// <summary>
  /// Converts JUCE MIDI buffer to Eden MIDI buffer.
  /// </summary>
  /// <param name="juceMidiBuffer">buffer to get messages from</param>
  /// <param name="edenMidiBuffer">buffer to pass messages to</param>
  static eden::MidiBuffer convertToEdenMidi(
      const juce::MidiBuffer& juceMidiBuffer);

  void addEdenParameters(AudioProcessorValueTreeState& pluginParameters);
  void updateEdenParameters(
      const AudioProcessorValueTreeState& pluginParameters);

  const WaveTablePathProvider& getPathProvider() const;
  std::shared_ptr<WaveshapingTransferFunctionContainer>
  getWaveshapingTransferFunction() const noexcept;

private:
  eden::EdenSynthesiser& _synthesiser;
  OscillatorContainer _oscillators;
  EdenFilterParameters _filterParameters;
  EdenWaveshapingParameters _waveshapingParameters;
  std::shared_ptr<eden::ADBDRParameters> _envelopeParameters{
      std::make_shared<eden::ADBDRParameters>()};
};
}  // namespace eden::plugin
