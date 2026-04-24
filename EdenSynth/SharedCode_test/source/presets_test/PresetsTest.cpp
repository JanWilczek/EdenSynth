#include <memory>
#include <vector>
#include <ranges>
#include <gtest/gtest.h>
#include <wolfsound/juce/wolfsound_ParameterHolder.hpp>
#include <wolfsound/test/wolfsound_TestAudioProcessorBase.hpp>
#include <wolfsound/common/wolfsound_WhenLeavingScopeExecute.hpp>
#include "parameters/SerializedParameters.h"
#include <PresetLoadingResult.h>
#include <utility/EdenAssert.h>
#include "../TestUtils.h"

namespace eden::plugin {
using PresetId = std::string;

struct PresetMetadata {
  static constexpr auto currentPresetVersion = 1;

  std::string name;
  bool isFactory;
  PresetId id;
  int presetVersion = currentPresetVersion;
};

class PresetV2 {
public:
  PresetV2(PresetMetadata metadata, SerializedParameters parameters)
      : _metadata{std::move(metadata)}, _parameters{std::move(parameters)} {}
  [[nodiscard]] const SerializedParameters& parameters() const {
    return _parameters;
  }
  [[nodiscard]] const std::string& name() const { return _metadata.name; }
  [[nodiscard]] bool isFactory() const noexcept { return _metadata.isFactory; }
  [[nodiscard]] const PresetId& id() const noexcept { return _metadata.id; }
  [[nodiscard]] const PresetMetadata& metadata() const noexcept {
    return _metadata;
  }

private:
  PresetMetadata _metadata;
  SerializedParameters _parameters;
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
    if constexpr (!std::is_const_v<T>) {
      metadata.presetVersion = archive.getVersion().value();
    }
    archive(named("id", metadata.id), named("name", metadata.name));
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

  const auto parameters = SerializedParameters::fromChecked(presetData);
  if (!parameters.has_value()) {
    return std::unexpected{PresetLoadingError::InvalidFile};
  }

  return PresetV2{presetMetadata.value(), parameters.value()};
}
}  // namespace

class PresetsRepository {
public:
  virtual ~PresetsRepository() = default;

  virtual std::optional<PresetV2> findPreset(const PresetId& presetId) = 0;
  virtual void savePreset(PresetV2) = 0;
  virtual std::vector<PresetV2> presets() = 0;
};

template <class VisitorBase>
void update(wolfsound::ParameterHolder<VisitorBase>& parameterHolder,
            const SerializedParameters& parameters) {
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

  bool loadPreset(const PresetId& presetId) {
    if (const auto preset = _presetsRepository->findPreset(presetId)) {
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
        SerializedParameters::fromChecked(wolfsound::toVarArray(_parameters));

    if (!parameters.has_value()) {
      EDEN_ASSERT(false);
      return false;
    }

    // as this is a brand-new preset, create an ID for it
    // to overwrite existing presets use updatePreset()
    presetMetadata.id = juce::Uuid{}.toDashedString().toStdString();

    _presetsRepository->savePreset(
        PresetV2{presetMetadata, parameters.value()});

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
  virtual ~UserPresetsDataSource() = default;
  virtual std::vector<PresetV2> presets() = 0;
  virtual void createPreset(const PresetV2& preset) = 0;
  //  void readPreset();
  //  void updatePreset();
  //  void deletePreset();
};

class FactoryPresetsDataSource {
public:
  virtual ~FactoryPresetsDataSource() = default;
  // factory presets can only be read, they cannot be modified
  virtual std::vector<PresetV2> presets() = 0;
};

namespace {
std::vector<PresetV2> scanDirectoryForPresets(const auto& directoryPath,
                                              bool isFactory) {
  using namespace std::filesystem;

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
}
}  // namespace

// implementers
class FileFactoryPresetsDataSource : public FactoryPresetsDataSource {
public:
  FileFactoryPresetsDataSource(std::filesystem::path factoryPresetsPath)
      : _factoryPresetsPath{std::move(factoryPresetsPath)} {}

  std::vector<PresetV2> presets() override {
    return scanDirectoryForPresets(_factoryPresetsPath, true);
  }

private:
  std::filesystem::path _factoryPresetsPath;
};

namespace {
/// <summary>
/// Update o1 with all properties from o2
/// </summary
void merge(juce::DynamicObject& o1, const juce::DynamicObject& o2) {
  for (const auto& property : o2.getProperties()) {
    o1.setProperty(property.name, property.value);
  }
}
}  // namespace

class FileUserPresetsDataSource : public UserPresetsDataSource {
public:
  static std::string filenameFrom(const std::string& presetName) {
    auto presetFilename =
        juce::File::createLegalFileName(presetName).toStdString() + ".json";
    auto toLower = [](char c) {
      return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    };
    std::ranges::transform(presetFilename, presetFilename.begin(), toLower);
    std::ranges::replace(presetFilename, ' ', '_');
    return presetFilename;
  }

  explicit FileUserPresetsDataSource(std::filesystem::path userPresetsPath)
      : _userPresetsPath{std::move(userPresetsPath)} {}

  std::vector<PresetV2> presets() override {
    return scanDirectoryForPresets(_userPresetsPath, false);
  }

  /// <summary>
  /// Write given preset a a JSON to disk
  /// </summary>
  void createPreset(const PresetV2& preset) override {
    // const auto safeFilename = findSafeFilename(safeFilename);
    const juce::File file{
        (_userPresetsPath / filenameFrom(preset.name())).string()};

    if (file.existsAsFile()) {
      // use updatePreset() to update an existing preset
      EDEN_ASSERT(false);
      return;
    }

    const auto maybeJson = presetToJson(preset);
    if (!maybeJson.has_value()) {
      return;
    }
    const auto& json = maybeJson.value();

    file.create();
    juce::FileOutputStream outputStream{file};
    if (outputStream.openedOk()) {
      outputStream.setPosition(0);
      outputStream.truncate();
      juce::JSON::writeToStream(
          outputStream, json,
          juce::JSON::FormatOptions{}
              .withIndentLevel(2)
              .withMaxDecimalPlaces(2)
              .withSpacing(juce::JSON::Spacing::multiLine));

      outputStream.flush();
    }
  }

private:
  static std::optional<juce::var> presetToJson(const PresetV2& preset) {
    auto maybeJson = juce::ToVar::convert(preset.metadata());

    if (!maybeJson.has_value()) {
      return {};
    }

    auto& json = maybeJson.value();
    const auto parametersVar = preset.parameters().toVar();
    EDEN_ASSERT(json.isObject());
    EDEN_ASSERT(parametersVar.isObject());
    merge(*json.getDynamicObject(), *parametersVar.getDynamicObject());

    return json;
  }

  std::filesystem::path _userPresetsPath;
};

// Which class should access the disk?
// Which class should combine factory and user presets?
// Or is the disk connection necessary? Maybe we can read the presets
// on startup and that's it?
class ProductionPresetsRepository : public PresetsRepository {
public:
  explicit ProductionPresetsRepository(
      FactoryPresetsDataSource& factoryPresetsDataSource,
      std::unique_ptr<UserPresetsDataSource> userPresetsDataSource)
      : _presets{factoryPresetsDataSource.presets()},
        _userPresetsDataSource{std::move(userPresetsDataSource)} {
    EDEN_ASSERT(_userPresetsDataSource != nullptr);
  }

  std::optional<PresetV2> findPreset(const PresetId& presetId) override {
    const auto presetIt = std::ranges::find_if(
        _presets, [&](const auto& preset) { return preset.id() == presetId; });

    if (presetIt != _presets.end()) {
      return *presetIt;
    }

    return {};
  }

  void savePreset(PresetV2 preset) override {
    _presets.push_back(std::move(preset));
  }

  std::vector<PresetV2> presets() override {
    auto presets = _userPresetsDataSource->presets();
    presets.reserve(presets.size() + _presets.size());
    presets.insert(presets.end(), _presets.begin(), _presets.end());
    return presets;
  }

private:
  std::vector<PresetV2> _presets;
  std::unique_ptr<UserPresetsDataSource> _userPresetsDataSource;
};

namespace {
class EmptyFactoryPresetsDataSource : public FactoryPresetsDataSource {
public:
  std::vector<PresetV2> presets() override { return {}; }
};

static EmptyFactoryPresetsDataSource emptyFactoryPresetsDataSource;

class FakeUserPresetsDataSource : public UserPresetsDataSource {
public:
  std::vector<PresetV2> presets() override { return presetsToReturn; }

  void createPreset(const PresetV2& preset) override {
    juce::ignoreUnused(preset);
  }

  std::vector<PresetV2> presetsToReturn;
};
}  // namespace

TEST(Presets, CanSavePreset) {
  PluginProcessorWithPresets processor{
      std::make_unique<ProductionPresetsRepository>(
          emptyFactoryPresetsDataSource,
          std::make_unique<FakeUserPresetsDataSource>())};

  ASSERT_TRUE(processor.savePreset(PresetMetadata{
      .name = "default",
      .isFactory = false,
  }));

  const auto presets = processor.presets();

  EXPECT_EQ(1u, presets.size());
}

TEST(Presets, CanLoadPreset) {
  PluginProcessorWithPresets processor{
      std::make_unique<ProductionPresetsRepository>(
          emptyFactoryPresetsDataSource,
          std::make_unique<FakeUserPresetsDataSource>())};
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

  const auto presets = processor.presets();
  ASSERT_EQ(1u, presets.size());
  EXPECT_EQ("min", presets.front().name());
  EXPECT_FALSE(presets.front().id().empty());
  EXPECT_TRUE(processor.loadPreset(presets.front().id()));

  EXPECT_FLOAT_EQ(1.f, processor.floatParam.get());
  EXPECT_FALSE(processor.boolParam.get());
  EXPECT_EQ(5, processor.intParam.get());
  EXPECT_EQ("choice 0",
            processor.choiceParam.getCurrentChoiceName().toStdString());
}

TEST(Presets, CannotLoadNonexistingPreset) {
  PluginProcessorWithPresets processor{
      std::make_unique<ProductionPresetsRepository>(
          emptyFactoryPresetsDataSource,
          std::make_unique<FakeUserPresetsDataSource>())};
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

namespace {
const auto& factoryPresetsPath() {
  static const auto result = testAssetsPath() / "factory_presets";
  return result;
}

const auto& userPresetsPath() {
  static const auto result = testAssetsPath() / "user_presets";
  return result;
}
}  // namespace

TEST(Presets, CanLoadFactoryPresetUponStart) {
  FileFactoryPresetsDataSource factoryPresetsDataSource{factoryPresetsPath()};
  PluginProcessorWithPresets processor{
      std::make_unique<ProductionPresetsRepository>(
          factoryPresetsDataSource,
          std::make_unique<FakeUserPresetsDataSource>())};

  ASSERT_EQ(1u, processor.presets().size());
  EXPECT_TRUE(processor.presets().front().isFactory());
  EXPECT_TRUE(processor.loadPreset("min-preset-id"));

  EXPECT_FLOAT_EQ(1.f, processor.floatParam.get());
  EXPECT_FALSE(processor.boolParam.get());
  EXPECT_EQ(5, processor.intParam.get());
  EXPECT_EQ("choice 0",
            processor.choiceParam.getCurrentChoiceName().toStdString());
}

TEST(ProductionPresetsRepository, ScansUserAndFactoryPresetsUponStart) {
  FileFactoryPresetsDataSource factoryPresetsDataSource{factoryPresetsPath()};
  auto userPresetsDataSource = std::make_unique<FakeUserPresetsDataSource>();
  userPresetsDataSource->presetsToReturn.emplace_back(
      PresetMetadata{
          .isFactory = false,
          .id = "user-preset-1",
      },
      SerializedParameters::fromChecked(juce::Array<juce::var>{}).value());
  userPresetsDataSource->presetsToReturn.emplace_back(
      PresetMetadata{
          .isFactory = false,
          .id = "user-preset-2",
      },
      SerializedParameters::fromChecked(juce::Array<juce::var>{}).value());
  ProductionPresetsRepository testee{factoryPresetsDataSource,
                                     std::move(userPresetsDataSource)};

  const auto& presets = testee.presets();

  EXPECT_EQ(3u, presets.size());
  EXPECT_TRUE(
      std::ranges::contains(presets, "user-preset-1",
                            [](const auto& preset) { return preset.id(); }));
  EXPECT_TRUE(
      std::ranges::contains(presets, "user-preset-2",
                            [](const auto& preset) { return preset.id(); }));
}

TEST(FileUserPresetsDataSource, SavesAndLoadsPresetsToDisk) {
  const wolfsound::WhenLeavingScopeExecute cleanup{[&] {
    std::for_each(std::filesystem::directory_iterator{userPresetsPath()},
                  std::filesystem::directory_iterator{}, [](const auto& entry) {
                    if (entry.path().extension() == ".json") {
                      std::filesystem::remove(entry.path());
                    }
                  });
  }};

  const PresetV2 presetToSave{
      PresetMetadata{
          .name = "User Preset 1",
          .isFactory = false,
          .id = "user-preset-1",
      },
      SerializedParameters::fromChecked(
          juce::Array{juce::JSON::fromString(R"({"id":"param1","value":10})")})
          .value()};
  {
    FileUserPresetsDataSource testee{userPresetsPath()};
    testee.createPreset(presetToSave);
  }

  FileUserPresetsDataSource testee{userPresetsPath()};
  const auto presets = testee.presets();

  ASSERT_EQ(1u, presets.size());
  const auto& savedPreset = presets.front();
  EXPECT_EQ(presetToSave.id(), savedPreset.id());
  EXPECT_EQ(presetToSave.name(), savedPreset.name());
  EXPECT_FALSE(savedPreset.isFactory());

  const auto toSaveParameterArray = presetToSave.parameters().toVarArray();
  const auto savedParameterArray = savedPreset.parameters().toVarArray();
  // compare parameter arrays as strings, because DynamicObject comparison
  // compares memory addresses
  EXPECT_EQ(juce::JSON::toString(toSaveParameterArray),
            juce::JSON::toString(savedParameterArray));
}

TEST(FileUserPresetsDataSource, SanitizesFilename) {
  EXPECT_EQ("user_preset_1.json",
            FileUserPresetsDataSource::filenameFrom("User Preset 1"));
  EXPECT_EQ("user_preset%!$_2.json",
            FileUserPresetsDataSource::filenameFrom("User Preset%!@#$ 2"));
}
}  // namespace eden::plugin