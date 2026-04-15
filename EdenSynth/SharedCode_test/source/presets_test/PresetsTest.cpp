#include <memory>
#include <vector>
#include <gtest/gtest.h>
#include <wolfsound/juce/wolfsound_ParameterHolder.hpp>
#include <wolfsound/test/wolfsound_TestAudioProcessorBase.hpp>
#include <parameters/Parameters.h>

namespace eden::plugin {
class PresetMetadata {
public:
  PresetMetadata(std::string name) : _name{std::move(name)} {}

private:
  std::string _name;
  //  bool _isFactory;
};

class PresetV2 {
public:
  PresetV2(PresetMetadata metadata, Parameters parameters)
      : _metadata{std::move(metadata)}, _parameters{std::move(parameters)} {}
  Parameters parameters() const { return _parameters; }

private:
  PresetMetadata _metadata;
  Parameters _parameters;
};

class PresetsRepository {
public:
  virtual ~PresetsRepository() = default;

  virtual std::optional<PresetV2> getPreset(std::string_view presetName) = 0;
  virtual void savePreset(PresetV2) = 0;
  virtual std::vector<PresetV2> presets() = 0;
};

template <class VisitorBase>
void update(wolfsound::ParameterHolder<VisitorBase>& parameterHolder,
            const Parameters& parameters) {
  update(parameterHolder, parameters.toVarArray());
}

class PluginProcessorWithPresets : public wolfsound::TestAudioProcessorBase {
public:
  explicit PluginProcessorWithPresets(
      std::unique_ptr<PresetsRepository> presetRepository,
      wolfsound::JuceParameterHolder::Builder builder = {})
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
        _parameters{std::move(builder).build(*this)},
        _presetsRepository{std::move(presetRepository)} {}

  bool loadPreset(std::string_view name) {
    if (const auto preset = _presetsRepository->getPreset(name)) {
      // TODO:
      const auto parameters = preset->parameters();
      update(_parameters, parameters);
      // _currentPresetName = preset.name();
      // _isPresetModified = false; // should add an asterisk in UI if true
      return true;
    }

    return false;
  }

  bool savePreset(PresetMetadata presetMetadata) {
    // check if parameter with same name exists?

    const auto parameters =
        Parameters::from(wolfsound::toVarArray(_parameters));

    // add some more metadata?

    _presetsRepository->savePreset(PresetV2{presetMetadata, parameters});

    return true;
  }

  std::vector<PresetV2> presets() { return _presetsRepository->presets(); }

  // parameters are public to enable observation (much like APVTS)
  juce::AudioParameterFloat& floatParam;
  juce::AudioParameterBool& boolParam;
  juce::AudioParameterInt& intParam;
  juce::AudioParameterChoice& choiceParam;

private:
  wolfsound::JuceParameterHolder _parameters;
  std::unique_ptr<PresetsRepository> _presetsRepository;
};

// interfaces
class UserPresetsDataSource {
public:
  void readPreset();
  void createPreset();
  void updatePreset();
  void deletePreset();
};

class FactoryPresetsDataSource {
public:
  void readPreset();
};

// implementers
class FileFactoryPresetsDataSource : public FactoryPresetsDataSource {};
class FileUserPresetsDataSource : public UserPresetsDataSource {};

// Which class should access the disk?
// Which class should combine factory and user presets?
// Or is the disk connection necessary? Maybe we can read the presets
// on startup and that's it?
class ProductionPresetsRepository : public PresetsRepository {
public:
  std::optional<PresetV2> getPreset(std::string_view presetName) override {
    // is factory? -> implies calling this with Preset not just the name
    juce::ignoreUnused(presetName);
    return {};
  }

  void savePreset(PresetV2 preset) override {
    _presets.push_back(std::move(preset));
  }

  std::vector<PresetV2> presets() override { return _presets; }

private:
  std::vector<PresetV2> _presets;
};

TEST(Presets, CanSavePreset) {
  PluginProcessorWithPresets processor{
      std::make_unique<ProductionPresetsRepository>()};

  ASSERT_TRUE(processor.savePreset(PresetMetadata{"default"}));

  const auto presets = processor.presets();

  EXPECT_EQ(1u, presets.size());
}

TEST(Presets, CanLoadPreset) {
  PluginProcessorWithPresets processor{
      std::make_unique<ProductionPresetsRepository>()};
  processor.floatParam = 1.f;
  processor.boolParam = false;
  processor.intParam = 5;
  processor.choiceParam = 0;

  ASSERT_TRUE(processor.savePreset(PresetMetadata{"min"}));

  processor.floatParam = 10.f;
  processor.boolParam = true;
  processor.intParam = 10;
  processor.choiceParam = 2;

  processor.loadPreset("min");

  EXPECT_FLOAT_EQ(1.f, processor.floatParam.get());
  EXPECT_FALSE(processor.boolParam.get());
  EXPECT_EQ(5, processor.intParam.get());
  EXPECT_EQ("choice 0",
            processor.choiceParam.getCurrentChoiceName().toStdString());
}
}  // namespace eden::plugin