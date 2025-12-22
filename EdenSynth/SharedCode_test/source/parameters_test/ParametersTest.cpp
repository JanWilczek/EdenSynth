#include <parameters/Parameters.h>
#include <gtest/gtest.h>

namespace eden::plugin {
/**
 * TODO TESTS:
 * - Update parameters
 * - toVar conversion
 * - construction from APVTS/ValueTree
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

  Parameters parameters{parameterTree};
  const auto parametersVar = parameters.toVar();

  ASSERT_TRUE(parametersVar.isObject());

  const auto serializedParameters = parametersVar["parameters"];
  ASSERT_TRUE(serializedParameters.isArray());
  ASSERT_EQ(46, serializedParameters.getArray()->size());
}
}  // namespace eden::plugin
