#pragma once
#include "eden/EdenSynthesiser.h"
#include <filesystem>
#include "JuceHeader.h"
#include "EdenAdapter.h"
#include "WaveTablePathProvider.h"
#include "PresetManager.h"
#include "parameters/ParameterRefs.h"
#include <wolfsound/juce/wolfsound_ParameterHolder.hpp>

class EdenSynthAudioProcessor : public AudioProcessor {
public:
  explicit EdenSynthAudioProcessor(
      wolfsound::JuceParameterHolder::Builder parameterBuilder = {});

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const String getProgramName(int index) override;
  void changeProgramName(int index, const String& newName) override;

  //==============================================================================
  void getStateInformation(MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  //==============================================================================
  [[nodiscard]] eden::plugin::PresetManager& getPresetManager() noexcept;
  [[nodiscard]] const eden::plugin::ParameterRefs& pluginParametersV2();

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EdenSynthAudioProcessor)

  eden::EdenSynthesiser _edenSynthesiser;
  eden::plugin::EdenAdapter _edenAdapter;
  wolfsound::JuceParameterHolder _pluginParametersV2;
  std::unique_ptr<eden::plugin::PresetManager> _presetManager;
};
