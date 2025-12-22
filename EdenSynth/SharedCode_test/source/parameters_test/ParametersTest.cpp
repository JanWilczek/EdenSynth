#include <parameters/Parameters.h>
#include <gtest/gtest.h>

namespace eden::plugin {
/**
 * TODO TESTS:
 * - Update parameters
 */

constexpr auto textXml = R"(
<?xml version="1.0" encoding="UTF-8"?>

<EdenSynthParameters>
  <PARAM id="envelope.adbdr.attack.curve" value="1.0"/>
  <PARAM id="envelope.adbdr.attack.time" value="30.0"/>
  <PARAM id="envelope.adbdr.breakLevel" value="0.6000000238418579"/>
  <PARAM id="envelope.adbdr.decay1.curve" value="1.0"/>
  <PARAM id="envelope.adbdr.decay1.time" value="20.0"/>
  <PARAM id="envelope.adbdr.decay2.curve" value="1.0"/>
  <PARAM id="envelope.adbdr.decay2.time" value="20000.0"/>
  <PARAM id="envelope.adbdr.release.curve" value="1.0"/>
  <PARAM id="envelope.adbdr.release.time" value="300.0"/>
  <PARAM id="filter.contourAmount" value="1.0"/>
  <PARAM id="filter.cutoff" value="1.0"/>
  <PARAM id="filter.env.adsr.attack.time" value="50.0"/>
  <PARAM id="filter.env.adsr.decay.time" value="20.0"/>
  <PARAM id="filter.env.adsr.release.time" value="300.0"/>
  <PARAM id="filter.env.adsr.sustain.level" value="0.9000000357627869"/>
  <PARAM id="filter.passbandAttenuation" value="0.0"/>
  <PARAM id="filter.resonance" value="0.0"/>
  <PARAM id="frequencyOfA4" value="440.0"/>
  <PARAM id="gen.osc1.centTransposition" value="0.0"/>
  <PARAM id="gen.osc1.generatorName" value="0.0"/>
  <PARAM id="gen.osc1.isRealTime" value="0.0"/>
  <PARAM id="gen.osc1.octaveTransposition" value="0.0"/>
  <PARAM id="gen.osc1.on" value="1.0"/>
  <PARAM id="gen.osc1.semitoneTransposition" value="0.0"/>
  <PARAM id="gen.osc1.volume" value="1.0"/>
  <PARAM id="gen.osc1.waveTable" value="9.0"/>
  <PARAM id="gen.osc2.centTransposition" value="0.0"/>
  <PARAM id="gen.osc2.generatorName" value="0.0"/>
  <PARAM id="gen.osc2.isRealTime" value="0.0"/>
  <PARAM id="gen.osc2.octaveTransposition" value="0.0"/>
  <PARAM id="gen.osc2.on" value="1.0"/>
  <PARAM id="gen.osc2.semitoneTransposition" value="0.0"/>
  <PARAM id="gen.osc2.volume" value="1.0"/>
  <PARAM id="gen.osc2.waveTable" value="9.0"/>
  <PARAM id="gen.osc3.centTransposition" value="0.0"/>
  <PARAM id="gen.osc3.generatorName" value="0.0"/>
  <PARAM id="gen.osc3.isRealTime" value="0.0"/>
  <PARAM id="gen.osc3.octaveTransposition" value="0.0"/>
  <PARAM id="gen.osc3.on" value="1.0"/>
  <PARAM id="gen.osc3.semitoneTransposition" value="0.0"/>
  <PARAM id="gen.osc3.volume" value="1.0"/>
  <PARAM id="gen.osc3.waveTable" value="9.0"/>
  <PARAM id="output.volume" value="1.0"/>
  <PARAM id="pitchBend.semitonesDown" value="-12.0"/>
  <PARAM id="pitchBend.semitonesUp" value="2.0"/>
  <PARAM id="waveshaper.autoMakeUpGain" value="0.0"/>
</EdenSynthParameters>
  )";

TEST(Parameters, CreateFromValueTree) {
  const auto parameterTree = juce::ValueTree::fromXml(juce::String{textXml});
  ASSERT_EQ(46, parameterTree.getNumChildren());

  const auto parameters = Parameters::from(parameterTree);
  const auto parametersVar = parameters.toVar();

  ASSERT_TRUE(parametersVar.isObject());

  const auto& serializedParameters = parametersVar["parameters"];
  ASSERT_TRUE(serializedParameters.isArray());
  ASSERT_EQ(46, serializedParameters.getArray()->size());
}

namespace {
class TestAudioProcessor : juce::AudioProcessor {
public:
  const String getName() const override { return "TestAudioProcessor"; }
  void prepareToPlay(double, int) override {}
  void releaseResources() override {}
  void processBlock(AudioBuffer<float>&, MidiBuffer&) override {}
  double getTailLengthSeconds() const override { return 0.0; }
  bool acceptsMidi() const override { return false; }
  bool producesMidi() const override { return false; }
  AudioProcessorEditor* createEditor() override { return nullptr; }
  bool hasEditor() const override { return false; }
  int getNumPrograms() override { return 1; }
  int getCurrentProgram() override { return 0; }
  void setCurrentProgram(int) override {}
  const String getProgramName(int) override { return ""; }
  void changeProgramName(int, const String&) override {}
  void getStateInformation(juce::MemoryBlock&) override {}
  void setStateInformation(const void*, int) override {}

  juce::AudioProcessorValueTreeState state{
      *this,
      nullptr,
      "TestAudioProcessor",
      {std::make_unique<juce::AudioParameterFloat>(
          "floatParam",
          "Float Param",
          juce::NormalisableRange{1.f, 10.f},
          5.f)}};
};
}  // namespace

TEST(Parameters, CorrectlyUpdatesAPVTS) {
  juce::ScopedJuceInitialiser_GUI guiInitializer;
  TestAudioProcessor processor;
  ASSERT_FLOAT_EQ(5.f,
                  processor.state.getRawParameterValue("floatParam")->load());

  const auto parameters = Parameters::from(processor.state.copyState());
  auto serializedParameters = parameters.toVar();
  auto parametersArray = serializedParameters["parameters"].getArray();
  ASSERT_NE(nullptr, parametersArray);
  auto& floatParam = parametersArray->getReference(0);
  ASSERT_FLOAT_EQ(5.f, float{floatParam["value"]});

  floatParam.getDynamicObject()->setProperty("value", 8.);

  const auto newParameters = Parameters::from(serializedParameters);

  updateApvts(processor.state, newParameters);

  EXPECT_FLOAT_EQ(8.f,
                  processor.state.getRawParameterValue("floatParam")->load());
}
}  // namespace eden::plugin
