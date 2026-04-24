#pragma once
#include "eden/EdenSynthesiser.h"
#include <filesystem>
#include "JuceHeader.h"
#include "EdenAdapter.h"
#include "WaveTablePathProvider.h"
#include "PresetManager.h"
#include "parameters/ParameterRefs.h"
#include <wolfsound/juce/wolfsound_ParameterHolder.hpp>

namespace eden::plugin {
class EdenSynthAudioProcessor : public juce::AudioProcessor {
public:
  explicit EdenSynthAudioProcessor(
      wolfsound::JuceParameterHolder::Builder parameterBuilder = {});

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  //==============================================================================
  [[nodiscard]] PresetManager& getPresetManager() noexcept;
  [[nodiscard]] const ParameterRefs& pluginParametersV2();

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EdenSynthAudioProcessor)

  EdenSynthesiser _edenSynthesiser;
  EdenAdapter _edenAdapter;
  wolfsound::JuceParameterHolder _pluginParameters;
  std::unique_ptr<PresetManager> _presetManager;
};
}  // namespace eden::plugin
