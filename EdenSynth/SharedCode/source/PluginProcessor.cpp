#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "EdenAdapter.h"
#include "FileHelper.h"
#include "eden/AudioBuffer.h"
#include "eden/MidiBuffer.h"
#include "ProductionPresetManager.h"

#include <wolfsound/juce/wolfsound_SerializedParameters.hpp>
#include <ranges>
#include <utility/EdenAssert.h>

namespace eden::plugin {
EdenSynthAudioProcessor::EdenSynthAudioProcessor(
    wolfsound::JuceParameterHolder::Builder parameterBuilder)
    :
#ifndef JucePlugin_PreferredChannelConfigurations
      AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
                         ),
#endif
      _edenAdapter(_edenSynthesiser,
                   parameterBuilder,
                   FileHelper::assetsPath()),
      _pluginParameters{std::move(parameterBuilder).build(*this)},
      _presetManager{std::make_unique<ProductionPresetManager>(
          ProductionPresetManager::Args{
              .systemPresetsPath = FileHelper::systemPresetsPath(),
              .userPresetsPath = FileHelper::userPresetsPath(),
              .getSerializedState =
                  [this]() {
                    const auto parameters =
                        wolfsound::SerializedParameters::from(
                            wolfsound::toVarArray(_pluginParameters));
                    EDEN_ASSERT(parameters.has_value());
                    return parameters.value();
                  },
              .setSerializedState =
                  [this](const wolfsound::SerializedParameters& p) {
                    wolfsound::update(_pluginParameters, p.toVarArray());
                  },
          })} {
}

//==============================================================================
const String EdenSynthAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool EdenSynthAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool EdenSynthAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool EdenSynthAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double EdenSynthAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int EdenSynthAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int EdenSynthAudioProcessor::getCurrentProgram() {
  return 0;
}

void EdenSynthAudioProcessor::setCurrentProgram(int /*index*/) {}

const String EdenSynthAudioProcessor::getProgramName(int /*index*/) {
  return {};
}

void EdenSynthAudioProcessor::changeProgramName(int /*index*/,
                                                const String& /*newName*/) {}

//==============================================================================
void EdenSynthAudioProcessor::prepareToPlay(double sampleRate,
                                            int samplesPerBlock) {
  _edenSynthesiser.setSampleRate(static_cast<float>(sampleRate));
  _edenSynthesiser.setBlockLength(samplesPerBlock);
}

void EdenSynthAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EdenSynthAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != AudioChannelSet::stereo()) {
    return false;  // NOLINT
  }

  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void EdenSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer& midiMessages) {
  ScopedNoDenormals noDenormals;

  _edenAdapter.updateEdenParameters();

  eden::AudioBuffer edenAudioBuffer(
      const_cast<float**>(buffer.getArrayOfWritePointers()),
      getTotalNumOutputChannels(),
      static_cast<unsigned>(buffer.getNumSamples()));
  eden::MidiBuffer edenMidiBuffer =
      EdenAdapter::convertToEdenMidi(midiMessages);

  _edenSynthesiser.processInputBlock(edenAudioBuffer, edenMidiBuffer);
}

//==============================================================================
bool EdenSynthAudioProcessor::hasEditor() const {
  return true;
}

AudioProcessorEditor* EdenSynthAudioProcessor::createEditor() {
  return new EdenSynthAudioProcessorEditor(*this, _edenAdapter);
}

//==============================================================================
void EdenSynthAudioProcessor::getStateInformation(MemoryBlock& destData) {
  const auto serializedParameters = wolfsound::SerializedParameters::from(
      wolfsound::toVarArray(_pluginParameters));
  if (serializedParameters.has_value()) {
    juce::MemoryOutputStream memory{destData, true};
    juce::JSON::writeToStream(memory, serializedParameters->toVar());
  }
}

void EdenSynthAudioProcessor::setStateInformation(const void* data,
                                                  int sizeInBytes) {
  juce::MemoryInputStream inputStream{data, static_cast<size_t>(sizeInBytes),
                                      false};
  const auto deserializedParameters = juce::JSON::parse(inputStream);
  const auto parameters =
      wolfsound::SerializedParameters::from(deserializedParameters);
  if (parameters.has_value()) {
    wolfsound::update(_pluginParameters, parameters->toVarArray());
  }
}

PresetManager& EdenSynthAudioProcessor::getPresetManager() noexcept {
  return *_presetManager;
}

const ParameterRefs& EdenSynthAudioProcessor::pluginParametersV2() {
  return _edenAdapter.parameterRefs();
}
}  // namespace eden::plugin

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new eden::plugin::EdenSynthAudioProcessor{};
}
