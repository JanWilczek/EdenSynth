#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "EdenAdapter.h"
#include "FileHelper.h"
#include "eden/AudioBuffer.h"
#include "eden/MidiBuffer.h"
#include "ProductionPresetManager.h"

#include "parameters/Parameters.h"
#include "utility/StopWatchPrinter.h"
#include "utility/WaveFileReader.h"
#include <filesystem>
#include <ranges>

//==============================================================================
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
      _pluginParameters(*this, nullptr),
      _edenAdapter(_edenSynthesiser,
                   parameterBuilder,
                   eden::plugin::FileHelper::assetsPath()),
      _pluginParametersV2{std::move(parameterBuilder).build(*this)},
      _presetManager{std::make_unique<eden::plugin::ProductionPresetManager>(
          eden::plugin::ProductionPresetManager::Args{
              .systemPresetsPath =
                  eden::plugin::FileHelper::systemPresetsPath(),
              .userPresetsPath = eden::plugin::FileHelper::userPresetsPath(),
              .getSerializedState =
                  [this]() {
                    return eden::plugin::Parameters::from(
                        _pluginParameters.copyState());
                  },
              .setSerializedState =
                  [this](const eden::plugin::Parameters& p) {
                    eden::plugin::updateApvts(_pluginParameters, p);
                  },
          })} {
  _pluginParameters.state = ValueTree(Identifier("EdenSynthParameters"));
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

void EdenSynthAudioProcessor::processBlock(AudioBuffer<float>& buffer,
                                           MidiBuffer& midiMessages) {
  // time measurement
  // eden::utility::StopWatchPrinter stopWatch;

  ScopedNoDenormals noDenormals;

  _edenAdapter.updateEdenParameters(_pluginParameters);

  eden::AudioBuffer edenAudioBuffer(
      const_cast<float**>(buffer.getArrayOfWritePointers()),
      getTotalNumOutputChannels(),
      static_cast<unsigned>(buffer.getNumSamples()));
  eden::MidiBuffer edenMidiBuffer =
      eden::plugin::EdenAdapter::convertToEdenMidi(midiMessages);

  _edenSynthesiser.processInputBlock(edenAudioBuffer, edenMidiBuffer);
}

//==============================================================================
bool EdenSynthAudioProcessor::hasEditor() const {
  return true;
}

AudioProcessorEditor* EdenSynthAudioProcessor::createEditor() {
  return new EdenSynthAudioProcessorEditor(*this, _pluginParameters,
                                           _edenAdapter);
}

//==============================================================================
namespace {
constexpr auto stateSeparator = "%%%%%%%%%%%%%";
}

void EdenSynthAudioProcessor::getStateInformation(MemoryBlock& destData) {
  auto state = _pluginParameters.copyState();

  // TODO: Add waveshaping curve
  const std::unique_ptr<XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);

  destData.append(stateSeparator, strlen(stateSeparator));

  const auto serializedParameters = eden::plugin::Parameters::fromChecked(
      wolfsound::toVarArray(_pluginParametersV2));
  if (serializedParameters.has_value()) {
    juce::MemoryOutputStream memory{destData, true};
    juce::JSON::writeToStream(memory, serializedParameters->toVar());
  }
}

void EdenSynthAudioProcessor::setStateInformation(const void* data,
                                                  int sizeInBytes) {
  static const auto stateSeparatorSize =
      static_cast<int>(strlen(stateSeparator));
  std::optional<int> separatorStartPos;
  for (const auto pos : std::views::iota(0, sizeInBytes - stateSeparatorSize)) {
    if (memcmp(
            reinterpret_cast<const std::byte*>(data) + static_cast<size_t>(pos),
            stateSeparator, strlen(stateSeparator)) == 0) {
      separatorStartPos = pos;
      break;
    }
  }

  const auto xmlEndPos =
      separatorStartPos.has_value() ? separatorStartPos.value() : sizeInBytes;

  std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, xmlEndPos));

  if (xmlState) {
    DBG(xmlState->toString());
    if (xmlState->hasTagName(_pluginParameters.state.getType())) {
      _pluginParameters.replaceState(ValueTree::fromXml(*xmlState));
    }
  }

  if (separatorStartPos.has_value()) {
    const auto v2StartPos = separatorStartPos.value() + stateSeparatorSize;
    juce::MemoryInputStream inputStream{data, static_cast<size_t>(sizeInBytes),
                                        false};
    inputStream.skipNextBytes(v2StartPos);
    const auto deserializedParameters = juce::JSON::parse(inputStream);
    const auto parameters =
        eden::plugin::Parameters::fromChecked(deserializedParameters);
    if (parameters.has_value()) {
      DBG(juce::JSON::toString(deserializedParameters));
      wolfsound::update(_pluginParametersV2, parameters->toVarArray());
    }
  }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new EdenSynthAudioProcessor{};
}

[[nodiscard]] eden::plugin::PresetManager&
EdenSynthAudioProcessor::getPresetManager() noexcept {
  return *_presetManager;
}

[[nodiscard]] AudioProcessorValueTreeState&
EdenSynthAudioProcessor::getPluginParameters() noexcept {
  return _pluginParameters;
}

const eden::plugin::ParameterRefs&
EdenSynthAudioProcessor::pluginParametersV2() {
  return _edenAdapter.parameterRefs();
}
