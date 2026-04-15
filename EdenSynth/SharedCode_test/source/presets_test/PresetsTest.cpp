#include <memory>
#include <vector>
#include <filesystem>
#include <ranges>
#include <gtest/gtest.h>
#include <wolfsound/juce/wolfsound_ParameterHolder.hpp>
#include <wolfsound/test/wolfsound_TestAudioProcessorBase.hpp>
#include <parameters/Parameters.h>
#include <PresetLoadingResult.h>

namespace eden::plugin {
struct PresetMetadata {
  static constexpr auto currentPresetVersion = 1;

  std::string name;
  bool isFactory;
  int presetVersion = currentPresetVersion;
};

class PresetV2 {
public:
  PresetV2(PresetMetadata metadata, Parameters parameters)
      : _metadata{std::move(metadata)}, _parameters{std::move(parameters)} {}
  Parameters parameters() const { return _parameters; }
  const std::string& name() const { return _metadata.name; }
  bool isFactory() const noexcept { return _metadata.isFactory; }

private:
  PresetMetadata _metadata;
  Parameters _parameters;
};
}  // namespace eden::plugin

// serialization
template <>
struct juce::SerialisationTraits<eden::plugin::PresetMetadata> {
  static constexpr auto marshallingVersion =
      eden::plugin::PresetMetadata::currentPresetVersion;

  template <class Archive, class T>
  static void serialise(Archive& archive, T& metadata) {
    if (!archive.getVersion().has_value()) {
      // fail parsing (a workaround since there's no archive.fail() function)
      auto placeholder = 0;
      archive(named("__version__", placeholder));
      return;
    }
    metadata.presetVersion = archive.getVersion().value();
    archive(named("name", metadata.name));
  }
};

namespace eden::plugin {
namespace {
std::expected<PresetV2, PresetLoadingError> presetFrom(
    const std::filesystem::path& path,
    bool isFactory) {
  const auto presetFile = juce::File{path.c_str()};

  if (!presetFile.existsAsFile()) {
    return std::unexpected{PresetLoadingError::DoesNotExist};
  }

  if (!presetFile.hasReadAccess()) {
    return std::unexpected{PresetLoadingError::NoPermission};
  }

  juce::FileInputStream inputStream{presetFile};
  if (!inputStream.openedOk()) {
    return std::unexpected{PresetLoadingError::FailedToReadFile};
  }
  const auto presetData = juce::JSON::parse(inputStream);
  auto presetMetadata = juce::FromVar::convert<PresetMetadata>(presetData);

  if (!presetMetadata.has_value()) {
    return std::unexpected{PresetLoadingError::InvalidFile};
  }
  presetMetadata->isFactory = isFactory;

  const auto parameters = Parameters::fromChecked(presetData);
  if (!parameters.has_value()) {
    return std::unexpected{PresetLoadingError::InvalidFile};
  }

  return PresetV2{presetMetadata.value(), parameters.value()};
}
}  // namespace

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
      const auto parameters = preset->parameters();
      // TODO: Set all parameters to default values before updating
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
  virtual ~FactoryPresetsDataSource() = default;
  // factory presets can only be read, they cannot be modified
  virtual std::vector<PresetV2> getPresets() = 0;
};

// implementers
class FileFactoryPresetsDataSource : public FactoryPresetsDataSource {
public:
  FileFactoryPresetsDataSource(std::filesystem::path factoryPresetsPath)
      : _factoryPresetsPath{std::move(factoryPresetsPath)} {}

  std::vector<PresetV2> getPresets() override {
    using namespace std::filesystem;

    auto scanDirectory = [&](const auto& directoryPath, bool isFactory) {
      if (!exists(directoryPath) || !is_directory(directoryPath)) {
        return std::vector<PresetV2>{};
      }

      auto isJson = [](const auto& entry) {
        return entry.is_regular_file() && entry.path().extension() == ".json";
      };
      auto toPreset = [=](const auto& entry) {
        return presetFrom(entry.path(), isFactory);
      };
      auto hasValue = [](const auto& opt) { return opt.has_value(); };
      auto toValue = [](const auto& opt) { return opt.value(); };

      return std::ranges::subrange(directory_iterator{directoryPath},
                                   directory_iterator{}) |
             std::views::filter(isJson) | std::views::transform(toPreset) |
             std::views::filter(hasValue) | std::views::transform(toValue) |
             std::ranges::to<std::vector<PresetV2>>();
    };

    return scanDirectory(_factoryPresetsPath, true);
  }

private:
  std::filesystem::path _factoryPresetsPath;
};

class FileUserPresetsDataSource : public UserPresetsDataSource {};

// Which class should access the disk?
// Which class should combine factory and user presets?
// Or is the disk connection necessary? Maybe we can read the presets
// on startup and that's it?
class ProductionPresetsRepository : public PresetsRepository {
public:
  explicit ProductionPresetsRepository(
      std::unique_ptr<FactoryPresetsDataSource> factoryPresetsDataSource)
      : _presets{factoryPresetsDataSource
                     ? factoryPresetsDataSource->getPresets()
                     : std::vector<PresetV2>{}} {}

  std::optional<PresetV2> getPreset(std::string_view presetName) override {
    const auto presetIt =
        std::ranges::find_if(_presets, [presetName](const auto& preset) {
          return preset.name() == presetName;
        });

    if (presetIt != _presets.end()) {
      return *presetIt;
    }

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
      std::make_unique<ProductionPresetsRepository>(nullptr)};

  ASSERT_TRUE(processor.savePreset(PresetMetadata{
      .name = "default",
      .isFactory = false,
  }));

  const auto presets = processor.presets();

  EXPECT_EQ(1u, presets.size());
}

TEST(Presets, CanLoadPreset) {
  PluginProcessorWithPresets processor{
      std::make_unique<ProductionPresetsRepository>(nullptr)};
  processor.floatParam = 1.f;
  processor.boolParam = false;
  processor.intParam = 5;
  processor.choiceParam = 0;

  ASSERT_TRUE(
      processor.savePreset(PresetMetadata{.name = "min", .isFactory = false}));

  processor.floatParam = 10.f;
  processor.boolParam = true;
  processor.intParam = 10;
  processor.choiceParam = 2;

  EXPECT_TRUE(processor.loadPreset("min"));

  EXPECT_FLOAT_EQ(1.f, processor.floatParam.get());
  EXPECT_FALSE(processor.boolParam.get());
  EXPECT_EQ(5, processor.intParam.get());
  EXPECT_EQ("choice 0",
            processor.choiceParam.getCurrentChoiceName().toStdString());
}

TEST(Presets, CannotLoadNonexistingPreset) {
  PluginProcessorWithPresets processor{
      std::make_unique<ProductionPresetsRepository>(nullptr)};
  processor.floatParam = 1.f;
  processor.boolParam = false;
  processor.intParam = 5;
  processor.choiceParam = 0;

  EXPECT_FALSE(processor.loadPreset("min"));

  EXPECT_FLOAT_EQ(1.f, processor.floatParam.get());
  EXPECT_FALSE(processor.boolParam.get());
  EXPECT_EQ(5, processor.intParam.get());
  EXPECT_EQ("choice 0",
            processor.choiceParam.getCurrentChoiceName().toStdString());
}

TEST(Presets, CanLoadFactoryPresetUponStart) {
  const auto factoryPresetsPath = std::filesystem::path(__FILE__)
                                      .parent_path()
                                      .parent_path()
                                      .parent_path() /
                                  "assets" / "factory_presets";

  PluginProcessorWithPresets processor{
      std::make_unique<ProductionPresetsRepository>(
          std::make_unique<FileFactoryPresetsDataSource>(factoryPresetsPath))};

  EXPECT_EQ(1u, processor.presets().size());
  EXPECT_TRUE(processor.presets().front().isFactory());
  EXPECT_TRUE(processor.loadPreset("Min (Factory Preset)"));

  EXPECT_FLOAT_EQ(1.f, processor.floatParam.get());
  EXPECT_FALSE(processor.boolParam.get());
  EXPECT_EQ(5, processor.intParam.get());
  EXPECT_EQ("choice 0",
            processor.choiceParam.getCurrentChoiceName().toStdString());
}

// TEST(Presets, CannotUpdateFactoryPreset) {
// TEST(Presets, CanLoadExistingUserPresetUponStart) {
// TEST(Presets, SavesNewPresetToDisk) {
// TEST(Presets, CannotOverwriteExistingPreset) {
// TEST(Presets, CanUpdateExistingPreset) {
// TEST(Presets, CanDeleteUserPreset) {
// TEST(Presets, CannotDeleteFactoryPreset) {
// Tests related to AudioProcessor's "programs" (=factory presets)
// ViewModel tests
}  // namespace eden::plugin