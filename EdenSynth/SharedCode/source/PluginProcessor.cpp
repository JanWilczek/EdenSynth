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
#include "presets/Preset.h"

namespace eden::plugin {
namespace {
class UpdatingParameterVisitor : public wolfsound::JuceParameterVisitor {
  [[nodiscard]] decltype(auto) findParameterWithId(
      const std::string& id) const noexcept {
    return std::ranges::find_if(
        _parameters, [&](const ParameterIdAndValue& p) { return p.id == id; });
  }

public:
  explicit UpdatingParameterVisitor(const ParameterValues& parameters)
      : _parameters{parameters} {}

  void visit(AudioParameterBool& p) override { visitImpl<bool>(p); }

  void visit(AudioParameterFloat& p) override { visitImpl<float>(p); }

  void visit(AudioParameterInt& p) override { visitImpl<int>(p); }

  void visit(AudioParameterChoice& parameter) override {
    const auto it =
        findParameterWithId(parameter.getParameterID().toStdString());
    if (it != _parameters.end()) {
      if (const auto* const choiceName = get_if<std::string>(&(it->value))) {
        const auto choiceIndex = parameter.choices.indexOf(*choiceName);
        // don't "clamp" incorrect values, ignore them
        if (0 <= choiceIndex && choiceIndex < parameter.choices.size()) {
          parameter = choiceIndex;
        }
      }
    }
  }

private:
  template <typename T>
  void visitImpl(auto& parameter) const {
    const auto it =
        findParameterWithId(parameter.getParameterID().toStdString());
    if (it != _parameters.end()) {
      if (const auto value = get_if<T>(&(it->value))) {
        parameter = *value;
      }
    }
  }

  const ParameterValues& _parameters;

  JUCE_DECLARE_NON_MOVEABLE(UpdatingParameterVisitor)
};

void update(wolfsound::JuceParameterHolder& ph,
            const ParameterValues& parameters) {
  UpdatingParameterVisitor visitor{parameters};
  ph.accept(visitor);
}

class ParameterValuesExtractor : public wolfsound::JuceParameterVisitor {
public:
  ParameterValuesExtractor() = default;

  void visit(juce::AudioParameterFloat& parameter) override {
    visitImpl(parameter, parameter.get());
  }

  void visit(juce::AudioParameterBool& parameter) override {
    visitImpl(parameter, parameter.get());
  }

  void visit(juce::AudioParameterInt& parameter) override {
    visitImpl(parameter, parameter.get());
  }

  void visit(juce::AudioParameterChoice& parameter) override {
    visitImpl(parameter, parameter.getCurrentChoiceName().toStdString());
  }

  [[nodiscard]] ParameterValues result() const { return _result; }

private:
  template <class P, class V>
  void visitImpl(const P& parameter, V&& value) {
    _result.emplace_back(parameter.getParameterID().toStdString(),
                         std::forward<V>(value));
  }

  ParameterValues _result;

  JUCE_DECLARE_NON_MOVEABLE(ParameterValuesExtractor)
};

ParameterValues parameterIdsAndValues(wolfsound::JuceParameterHolder& ph) {
  ParameterValuesExtractor visitor;
  ph.accept(visitor);
  return visitor.result();
}
}  // namespace

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
  const auto parameters = parameterIdsAndValues(_pluginParameters);
  const auto serializedParameters = juce::ToVar::convert(parameters);
  if (serializedParameters.has_value()) {
    juce::MemoryOutputStream memory{destData, true};
    juce::JSON::writeToStream(memory, serializedParameters.value());
  }
}

void EdenSynthAudioProcessor::setStateInformation(const void* data,
                                                  int sizeInBytes) {
  juce::MemoryInputStream inputStream{data, static_cast<size_t>(sizeInBytes),
                                      false};
  const auto deserializedParameters = juce::JSON::parse(inputStream);
  const auto parameters = juce::FromVar::convert<ParameterValues>(
      deserializedParameters.hasProperty("parameters")
          ? deserializedParameters["parameters"]
          : deserializedParameters);
  if (parameters.has_value()) {
    update(_pluginParameters, parameters.value());
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
