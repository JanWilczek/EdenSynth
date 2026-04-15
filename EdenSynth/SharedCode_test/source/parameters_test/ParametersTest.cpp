#include <parameters/Parameters.h>
#include <gtest/gtest.h>
#include <functional>
#include <memory>
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"
#include <wolfsound/test/wolfsound_TestAudioProcessorBase.hpp>

namespace eden::plugin {
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
using ParameterLayout = juce::AudioProcessorValueTreeState::ParameterLayout;

template <typename Parameter, typename... Args>
Parameter& addToLayout(ParameterLayout& layout, Args&&... args) {
  auto param = std::make_unique<Parameter>(std::forward<Args>(args)...);
  auto& ref = *param;
  layout.add(std::move(param));
  return ref;
}

class ApvtsAudioProcessor : public wolfsound::TestAudioProcessorBase {
public:
  explicit ApvtsAudioProcessor(ParameterLayout layout = {})
      : floatParam{addToLayout<juce::AudioParameterFloat>(
            layout,
            "floatParam",
            "Float Param",
            juce::NormalisableRange{1.f, 10.f},
            5.f)},
        boolParam{addToLayout<juce::AudioParameterBool>(layout,
                                                        "boolParam",
                                                        "Bool Param",
                                                        true)},
        intParam{addToLayout<juce::AudioParameterInt>(layout,
                                                      "intParam",
                                                      "Int Param",
                                                      5,
                                                      10,
                                                      6)},
        choiceParam{addToLayout<juce::AudioParameterChoice>(
            layout,
            "choiceParam",
            "Choice Param",
            juce::StringArray{"choice 0", "choice 1", "choice 2"},
            1)},
        state{*this, nullptr, "ApvtsAudioProcessor", std::move(layout)} {}

  juce::AudioParameterFloat& floatParam;
  juce::AudioParameterBool& boolParam;
  juce::AudioParameterInt& intParam;
  juce::AudioParameterChoice& choiceParam;
  juce::AudioProcessorValueTreeState state;
};

class WhenLeavingScopeExecute {  // NOLINT
public:
  explicit WhenLeavingScopeExecute(std::function<void()> callback)
      : _callback{std::move(callback)} {}

  ~WhenLeavingScopeExecute() {
    if (_callback) {
      _callback();
    }
  }

private:
  std::function<void()> _callback;
};
}  // namespace

TEST(Parameters, CorrectlyUpdatesApvts) {
  // given
  const juce::ScopedJuceInitialiser_GUI guiInitializer;
  ApvtsAudioProcessor processor;

  const auto parameters = Parameters::from(processor.state.copyState());
  auto serializedParameters = parameters.toVar();

  DBG(juce::JSON::toString(
      serializedParameters,
      juce::JSON::FormatOptions{}.withIndentLevel(2).withMaxDecimalPlaces(3)));

  auto* parametersArray = serializedParameters["parameters"].getArray();
  ASSERT_NE(nullptr, parametersArray);
  ASSERT_EQ(4, parametersArray->size());

  // APVTS sorts the parameters, thus, they appear in a different order than
  // defined
  auto& floatParam = parametersArray->getReference(2);
  ASSERT_EQ("floatParam", floatParam["id"]);
  ASSERT_FLOAT_EQ(5.f, float{floatParam["value"]});

  auto& boolParam = parametersArray->getReference(0);
  ASSERT_EQ("boolParam", boolParam["id"]);
  ASSERT_TRUE(bool{boolParam["value"]});

  auto& intParam = parametersArray->getReference(3);
  ASSERT_EQ("intParam", intParam["id"]);
  ASSERT_EQ(6, int{intParam["value"]});

  auto& choiceParam = parametersArray->getReference(1);
  ASSERT_EQ("choiceParam", choiceParam["id"]);
  // mind you that choice parameter is stored as a double (as all other
  // parameters for that matter)
  ASSERT_EQ(1, int{choiceParam["value"]});

  // when
  floatParam.getDynamicObject()->setProperty("value", 8.);
  boolParam.getDynamicObject()->setProperty("value", false);
  intParam.getDynamicObject()->setProperty("value", 8);
  choiceParam.getDynamicObject()->setProperty("value", 2);

  // then
  const auto newParameters = Parameters::from(serializedParameters);

  updateApvts(processor.state, newParameters);

  EXPECT_FLOAT_EQ(8.f, processor.floatParam.get());
  EXPECT_FALSE(processor.boolParam.get());
  EXPECT_EQ(8, processor.intParam.get());
  EXPECT_EQ("choice 2",
            processor.choiceParam.getCurrentChoiceName().toStdString());
}

TEST(Parameters, CorrectlyRestoresStateFromFile) {
  // given
  const juce::ScopedJuceInitialiser_GUI guiInitializer;
  ApvtsAudioProcessor processor;
  const auto parameters = Parameters::from(processor.state.copyState());

  const auto presetFile =
      juce::File::getSpecialLocation(
          juce::File::SpecialLocationType::tempDirectory)
          .getChildFile("CorrectlyRestoresStateFromFile.json");
  const WhenLeavingScopeExecute deleteTemporaryPresetFile{
      [&] { presetFile.deleteFile(); }};
  {
    juce::FileOutputStream outputStream{presetFile};
    ASSERT_TRUE(outputStream.openedOk());
    outputStream.setPosition(0);
    outputStream.truncate();
    juce::JSON::writeToStream(outputStream, parameters.toVar(),
                              juce::JSON::FormatOptions{}
                                  .withIndentLevel(2)
                                  .withMaxDecimalPlaces(2)
                                  .withSpacing(juce::JSON::Spacing::multiLine));
  }

  processor.floatParam = 1.f;
  processor.boolParam = false;
  processor.intParam = 9;
  processor.choiceParam = processor.choiceParam.choices.indexOf("choice 2");

  // when
  juce::FileInputStream inputStream{presetFile};
  ASSERT_TRUE(inputStream.openedOk());
  const auto newParametersData = juce::JSON::parse(inputStream);
  const auto newParameters = Parameters::from(newParametersData);
  updateApvts(processor.state, newParameters);

  // then
  EXPECT_FLOAT_EQ(5.f, processor.floatParam.get());
  EXPECT_TRUE(processor.boolParam.get());
  EXPECT_EQ(6, processor.intParam.get());
  EXPECT_EQ("choice 1",
            processor.choiceParam.getCurrentChoiceName().toStdString());
}

TEST(Parameters, FromVarChecked) {
  const auto result = Parameters::fromChecked(juce::JSON::parse(R"-(
{
  "name": "Min (Factory Preset)",
  "parameters": [
  {
    "id": "floatParam",
        "value": 1.0
  },
  {
    "id": "boolParam",
        "value": false
  },
  {
    "id": "intParam",
        "value": 5
  },
  {
    "id": "choiceParam",
        "value": "choice 0"
  }
  ]
}
)-"));

  EXPECT_TRUE(result.has_value());
}
}  // namespace eden::plugin
