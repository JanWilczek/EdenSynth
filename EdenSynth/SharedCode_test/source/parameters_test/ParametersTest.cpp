#include <parameters/Parameters.h>
#include <gtest/gtest.h>
#include <functional>
#include <memory>
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"

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

class TestAudioProcessor : public juce::AudioProcessor {
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
};

class ApvtsAudioProcessor : public TestAudioProcessor {
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
        state{*this, nullptr, "ApvtsAudioProcessor", std::move(layout)} {}

  juce::AudioParameterFloat& floatParam;
  juce::AudioParameterBool& boolParam;
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

  auto* parametersArray = serializedParameters["parameters"].getArray();
  ASSERT_NE(nullptr, parametersArray);
  ASSERT_EQ(2, parametersArray->size());

  auto& floatParam = parametersArray->getReference(1);
  ASSERT_EQ("floatParam", floatParam["id"]);
  ASSERT_FLOAT_EQ(5.f, float{floatParam["value"]});

  auto& boolParam = parametersArray->getReference(0);
  ASSERT_EQ("boolParam", boolParam["id"]);
  ASSERT_TRUE(bool{boolParam["value"]});

  // when
  floatParam.getDynamicObject()->setProperty("value", 8.);
  boolParam.getDynamicObject()->setProperty("value", false);

  // then
  const auto newParameters = Parameters::from(serializedParameters);

  updateApvts(processor.state, newParameters);

  EXPECT_FLOAT_EQ(8.f, processor.floatParam.get());
  EXPECT_FALSE(processor.boolParam.get());
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

  // when
  juce::FileInputStream inputStream{presetFile};
  ASSERT_TRUE(inputStream.openedOk());
  const auto newParametersData = juce::JSON::parse(inputStream);
  const auto newParameters = Parameters::from(newParametersData);
  updateApvts(processor.state, newParameters);

  // then
  EXPECT_FLOAT_EQ(5.f, processor.floatParam.get());
  EXPECT_TRUE(processor.boolParam.get());
}

namespace {
template <class Visitor>
class TypeErasedParameter {
public:
  template <class Parameter>
  explicit TypeErasedParameter(Parameter& p)
      : _impl{std::make_unique<ParameterModel<Parameter>>(p)} {}

  /**
   * The Visitor class is expected to have a visit() member function
   * for each supported parameter type, e.g.,
   *
   *   struct Visitor {
   *     void visit(juce::AudioParameterBool&);
   *     void visit(juce::AudioParameterFloat&);
   *     //...
   *   };
   */
  void accept(Visitor& v) { _impl->accept(v); }

private:
  class ParameterConcept {  // NOLINT
  public:
    virtual ~ParameterConcept() = default;
    virtual void accept(Visitor& v) = 0;
  };

  template <class Parameter>
  class ParameterModel : public ParameterConcept {
  public:
    explicit ParameterModel(Parameter& p) : _p{p} {}

    void accept(Visitor& v) override { v.visit(_p.get()); }

  private:
    std::reference_wrapper<Parameter> _p;
  };

  std::unique_ptr<ParameterConcept> _impl;
};

template <class Visitor>
class ParameterHolder {
public:
  explicit ParameterHolder(std::vector<TypeErasedParameter<Visitor>> parameters)
      : _parameters{std::move(parameters)} {}

  void accept(Visitor& v) {
    for (auto& parameter : _parameters) {
      parameter.accept(v);
    }
  }

private:
  std::vector<TypeErasedParameter<Visitor>> _parameters;
};

template <class Visitor>
class ParameterHolderBuilder {
public:
  template <class P, class... Args>
  P& add(Args&&... args) {
    auto parameter = std::make_unique<P>(std::forward<Args>(args)...);
    auto& ref = *parameter;
    _parametersForHolder.emplace_back(ref);
    _parameters.push_back(std::move(parameter));
    return ref;
  }

  ParameterHolder<Visitor> build(juce::AudioProcessor& p) {
    for (auto&& parameter : _parameters) {
      p.addParameter(parameter.release());
    }
    return ParameterHolder{std::move(_parametersForHolder)};
  }

private:
  std::vector<std::unique_ptr<juce::RangedAudioParameter>> _parameters;
  std::vector<TypeErasedParameter<Visitor>> _parametersForHolder;
};

struct VisitorBase {
  VisitorBase() = default;
  virtual ~VisitorBase() = default;
  VisitorBase(const VisitorBase&) = delete;
  VisitorBase& operator=(const VisitorBase&) = delete;
  VisitorBase(VisitorBase&&) = delete;
  VisitorBase& operator=(VisitorBase&&) = delete;

  virtual void visit(juce::AudioParameterBool&) = 0;
  virtual void visit(juce::AudioParameterFloat&) = 0;
  virtual void visit(juce::AudioParameterInt&) = 0;
  virtual void visit(juce::AudioParameterChoice&) = 0;
};

class VarArrayVistior : public VisitorBase {
public:
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
    visitImpl(parameter, parameter.getCurrentChoiceName());
  }

  [[nodiscard]] juce::Array<juce::var> result() const { return _result; }

private:
  template <class P, class V>
  void visitImpl(P& parameter, V value) {
    const juce::DynamicObject::Ptr object{new juce::DynamicObject};
    object->setProperty("id", parameter.getParameterID());
    object->setProperty("value", std::move(value));
    _result.add(juce::var{object});
  }

  juce::Array<juce::var> _result;
};

class UpdatingVisitor : public VisitorBase {
public:
  explicit UpdatingVisitor(const juce::Array<juce::var>& parameters)
      : _parameters{parameters} {}

  void visit(juce::AudioParameterFloat& parameter) override {
    visitImpl(parameter, [&](const auto& v) {
      if (v.isDouble()) {
        parameter = static_cast<float>(v);
      }
    });
  }

  void visit(juce::AudioParameterBool& parameter) override {
    visitImpl(parameter, [&](const auto& v) {
      if (v.isBool()) {
        parameter = static_cast<bool>(v);
      }
    });
  }

  void visit(juce::AudioParameterInt& parameter) override {
    visitImpl(parameter, [&](const auto& v) {
      if (v.isInt()) {
        parameter = static_cast<int>(v);
      }
    });
  }

  void visit(juce::AudioParameterChoice& parameter) override {
    visitImpl(parameter, [&](const auto& v) {
      if (v.isString()) {
        const auto choiceName = v.toString();
        const auto choiceIndex = parameter.choices.indexOf(choiceName);
        // don't "clamp" incorrect values, ignore them
        if (0 <= choiceIndex && choiceIndex < parameter.choices.size()) {
          parameter = choiceIndex;
        }
      }
    });
  }

private:
  template <class P>
  void visitImpl(P& parameter,
                 std::function<void(const juce::var&)> assignment) {
    const auto it = std::ranges::find_if(_parameters, [&](const juce::var& p) {
      return p.hasProperty("id") && p["id"] == parameter.getParameterID();
    });
    if (it != _parameters.end() && it->hasProperty("value")) {
      const auto& value = (*it)["value"];
      assignment(value);
    }
  }

  const juce::Array<juce::var>& _parameters;
};

juce::Array<juce::var> toVarArray(ParameterHolder<VisitorBase>& ph) {
  VarArrayVistior visitor;
  ph.accept(visitor);
  return visitor.result();
}

void update(ParameterHolder<VisitorBase>& ph,
            const juce::Array<juce::var>& parameters) {
  UpdatingVisitor visitor{parameters};
  ph.accept(visitor);
}

class ParameterHolderAudioProcessor : public TestAudioProcessor {
public:
  explicit ParameterHolderAudioProcessor(
      ParameterHolderBuilder<VisitorBase> builder = {})
      : floatParam{builder.add<juce::AudioParameterFloat>(
            "floatParam",
            "Float Param",
            juce::NormalisableRange{1.f, 10.f},
            5.f)},
        boolParam{builder.add<juce::AudioParameterBool>("boolParam",
                                                        "Bool Param",
                                                        true)},
        intParam{builder.add<juce::AudioParameterInt>("intParam",
                                                      "Int Param",
                                                      5,
                                                      10,
                                                      6)},
        choiceParam{builder.add<juce::AudioParameterChoice>(
            "choiceParam",
            "Choice Param",
            juce::StringArray{"choice 0", "choice 1", "choice 2"},
            1)},
        parameterHolder{builder.build(*this)} {}

  void getStateInformation(juce::MemoryBlock& block) override {
    const auto params = toVarArray(parameterHolder);
    juce::MemoryOutputStream output{block, true};
    juce::JSON::writeToStream(output, params);
  }

  void setStateInformation(const void* data, int size) override {
    juce::MemoryInputStream input{data, static_cast<size_t>(size), false};
    const auto parametersVar = juce::JSON::parse(input);
    jassert(parametersVar.isArray());
    update(parameterHolder, *parametersVar.getArray());
  }

  juce::AudioParameterFloat& floatParam;
  juce::AudioParameterBool& boolParam;
  juce::AudioParameterInt& intParam;
  juce::AudioParameterChoice& choiceParam;
  ParameterHolder<VisitorBase> parameterHolder;
};
}  // namespace

TEST(ParameterHolder, CorrectlyAddsParameters) {
  const ParameterHolderAudioProcessor processor;
  ASSERT_EQ(4u, processor.getParameters().size());
}

TEST(ParameterHolder, CorrectlyRestoresState) {
  juce::MemoryBlock state;

  {
    ParameterHolderAudioProcessor processor;
    processor.floatParam = 2.f;
    processor.boolParam = false;
    processor.intParam = 7;
    processor.choiceParam = 2;
    processor.getStateInformation(state);
  }
  DBG(state.toString());
  {
    ParameterHolderAudioProcessor processor;
    processor.setStateInformation(state.getData(),
                                  static_cast<int>(state.getSize()));

    EXPECT_FLOAT_EQ(2.f, processor.floatParam.get());
    EXPECT_FALSE(processor.boolParam.get());
    EXPECT_EQ(7, processor.intParam.get());
    EXPECT_EQ("choice 2",
              processor.choiceParam.getCurrentChoiceName().toStdString());
  }
}
}  // namespace eden::plugin
