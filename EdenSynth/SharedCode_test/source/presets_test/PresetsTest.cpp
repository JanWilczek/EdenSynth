#include <memory>
#include <vector>
#include <ranges>
#include <gtest/gtest.h>
#include <wolfsound/juce/wolfsound_ParameterHolder.hpp>
#include <wolfsound/test/wolfsound_TestAudioProcessorBase.hpp>
#include <wolfsound/common/wolfsound_WhenLeavingScopeExecute.hpp>
#include "presets/Preset.h"
#include "presets/PresetRepository.h"
#include "parameters/ParameterHolderUtils.h"
#include <PresetLoadingResult.h>
#include <utility/EdenAssert.h>
#include "../TestUtils.h"

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
  const auto presetJson = juce::JSON::parse(inputStream);
  auto presetData = juce::FromVar::convert<PresetData>(presetJson);

  if (!presetData.has_value()) {
    return std::unexpected{PresetLoadingError::InvalidFile};
  }
  presetData->metadata.isFactory = isFactory;

  return PresetV2{std::move(presetData.value())};
}
}  // namespace

class PluginProcessorWithPresets : public wolfsound::TestAudioProcessorBase {
public:
  explicit PluginProcessorWithPresets(
      std::unique_ptr<PresetRepository> presetRepository,
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

  void savePreset(PresetMetadata presetMetadata) {
    // check if parameter with same name exists?

    const auto parameters = parameterIdsAndValues(_parameters);

    // as this is a brand-new preset, create an ID for it
    // to overwrite existing presets use updatePreset()
    presetMetadata.id = juce::Uuid{}.toDashedString().toStdString();

    _presetsRepository->savePreset(PresetV2{presetMetadata, parameters});
  }

  std::vector<PresetV2> presets() { return _presetsRepository->presets(); }

  // parameters are public to enable observation (much like APVTS)
  juce::AudioParameterFloat& floatParam;
  juce::AudioParameterBool& boolParam;
  juce::AudioParameterInt& intParam;
  juce::AudioParameterChoice& choiceParam;

private:
  wolfsound::JuceParameterHolder _parameters;
  std::unique_ptr<PresetRepository> _presetsRepository;
};

// interfaces
class UserPresetsDataSource {
public:
  virtual ~UserPresetsDataSource() = default;
  virtual std::vector<PresetV2> presets() = 0;
  virtual void createPreset(const PresetV2&) = 0;
  virtual void updatePreset(const PresetV2&) = 0;
  virtual void deletePreset(const std::string& name) noexcept = 0;
  virtual bool contains(const PresetId&) = 0;
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
         std::ranges::to<std::vector>();
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

///< summary>
/// Preset file names are derived from preset names and must be unique.
/// If the user tries to save a preset with a name that already exists,
/// it should be rejected (ideally in the ViewModel).
///</summary>
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
    const juce::File file{
        (_userPresetsPath / filenameFrom(preset.name())).string()};

    if (file.existsAsFile()) {
      // use updatePreset() to update an existing preset
      EDEN_ASSERT(false);
      return;
    }

    saveOrOverwrite(preset, file);
  }

  void updatePreset(const PresetV2& preset) override {
    const juce::File file{
        (_userPresetsPath / filenameFrom(preset.name())).string()};

    EDEN_ASSERT(file.existsAsFile());

    saveOrOverwrite(preset, file);
  }

  void deletePreset(const std::string& name) noexcept override {
    EDEN_ASSERT(
        std::filesystem::directory_iterator{} !=
        std::ranges::find(std::filesystem::directory_iterator{_userPresetsPath},
                          filenameFrom(name), [](const auto& dirEntry) {
                            return dirEntry.path().filename().string();
                          }));
    std::error_code errorCode;
    std::filesystem::remove(_userPresetsPath / filenameFrom(name), errorCode);
    EDEN_ASSERT(!errorCode);
  }

  [[nodiscard]] bool contains(const PresetId& presetId) noexcept override {
    return std::ranges::contains(
        presets() | std::views::transform(&PresetV2::id), presetId);
  }

private:
  static std::optional<juce::var> presetToJson(const PresetV2& preset) {
    return juce::ToVar::convert(preset.data());
  }

  static void saveOrOverwrite(const PresetV2& preset, const juce::File& file) {
    const auto maybeJson = presetToJson(preset);
    if (!maybeJson.has_value()) {
      return;
    }
    const auto& json = maybeJson.value();

    file.create();
    juce::FileOutputStream outputStream{file};
    if (outputStream.openedOk()) {
      juce::JSON::writeToStream(
          outputStream, json,
          juce::JSON::FormatOptions{}
              .withIndentLevel(2)
              .withMaxDecimalPlaces(2)
              .withSpacing(juce::JSON::Spacing::multiLine));

      outputStream.flush();
    }
  }

  std::filesystem::path _userPresetsPath;
};

class ProductionPresetsRepository : public PresetRepository {
public:
  explicit ProductionPresetsRepository(
      FactoryPresetsDataSource& factoryPresetsDataSource,
      std::unique_ptr<UserPresetsDataSource> userPresetsDataSource)
      : _presets{factoryPresetsDataSource.presets()},
        _userPresetsDataSource{std::move(userPresetsDataSource)} {
    EDEN_ASSERT(_userPresetsDataSource != nullptr);
  }

  std::optional<PresetV2> findPreset(const PresetId& presetId) override {
    const auto currentPresets = presets();
    const auto presetIt = std::ranges::find_if(
        currentPresets,
        [&](const auto& preset) { return preset.id() == presetId; });

    if (presetIt != currentPresets.end()) {
      return *presetIt;
    }

    return {};
  }

  void savePreset(PresetV2 preset) override {
    if (_userPresetsDataSource->contains(preset.id())) {
      // overwrite
      _userPresetsDataSource->updatePreset(preset);
    } else {
      // optimistic update
      _presets.push_back(std::move(preset));

      _userPresetsDataSource->createPreset(_presets.back());
    }
  }

  std::vector<PresetV2> presets() override {
    auto presets = _userPresetsDataSource->presets();
    presets.reserve(presets.size() + _presets.size());
    presets.insert(presets.end(), _presets.begin(), _presets.end());
    return presets;
  }

  void deletePreset(const std::string& name) noexcept override {
    _userPresetsDataSource->deletePreset(name);
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

  void updatePreset(const PresetV2& preset) override {
    EDEN_ASSERT(contains(preset.id()));

    const auto it = std::ranges::find_if(
        presetsToReturn,
        [&preset](const auto& p) { return preset.id() == p.id(); });

    *it = preset;
  }

  bool contains(const PresetId& id) override {
    return std::ranges::contains(
        presetsToReturn, id, [](auto const& preset) { return preset.id(); });
  }

  void deletePreset(const std::string&) noexcept override {}

  std::vector<PresetV2> presetsToReturn;
};
}  // namespace

TEST(Presets, CanSavePreset) {
  PluginProcessorWithPresets processor{
      std::make_unique<ProductionPresetsRepository>(
          emptyFactoryPresetsDataSource,
          std::make_unique<FakeUserPresetsDataSource>())};

  processor.savePreset(PresetMetadata{
      .name = "default",
      .isFactory = false,
  });

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

  processor.savePreset(PresetMetadata{.name = "min", .isFactory = false});

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
      ParameterIdAndValueContainer{});
  userPresetsDataSource->presetsToReturn.emplace_back(
      PresetMetadata{
          .isFactory = false,
          .id = "user-preset-2",
      },
      ParameterIdAndValueContainer{});
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

TEST(ProductionPresetsRepository, SavesAndLoadsNewPreset) {
  auto userPresetsDataSource = std::make_unique<FakeUserPresetsDataSource>();
  ProductionPresetsRepository testee{emptyFactoryPresetsDataSource,
                                     std::move(userPresetsDataSource)};

  testee.savePreset(
      PresetV2{PresetMetadata{
                   .isFactory = false,
                   .id = "user-preset-1",
               },
               ParameterIdAndValueContainer{{.id = "parameter1", .value = 0}}});

  ASSERT_EQ(1u, testee.presets().size());
  const auto maybePreset = testee.findPreset("user-preset-1");
  ASSERT_TRUE(maybePreset.has_value());
  ASSERT_EQ(0, std::get<int>(maybePreset->parameters().front().value));
}

TEST(ProductionPresetsRepository, UpdatesAndLoadsExistingPreset) {
  auto userPresetsDataSource = std::make_unique<FakeUserPresetsDataSource>();
  auto userPreset =
      PresetV2{PresetMetadata{
                   .isFactory = false,
                   .id = "user-preset-1",
               },
               ParameterIdAndValueContainer{{.id = "parameter1", .value = 0}}};
  userPresetsDataSource->presetsToReturn.push_back(userPreset);
  ProductionPresetsRepository testee{emptyFactoryPresetsDataSource,
                                     std::move(userPresetsDataSource)};

  testee.savePreset(PresetV2{userPreset.metadata(),
                             ParameterIdAndValueContainer{{
                                 .id = userPreset.parameters().front().id,
                                 .value = 1,
                             }}});

  ASSERT_EQ(1u, testee.presets().size());
  const auto maybePreset = testee.findPreset("user-preset-1");
  ASSERT_TRUE(maybePreset.has_value());
  ASSERT_EQ(1, std::get<int>(maybePreset->parameters().front().value));
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

  const PresetV2 presetToSave{PresetMetadata{
                                  .name = "User Preset 1",
                                  .isFactory = false,
                                  .id = "user-preset-1",
                              },
                              {{.id = "param1", .value = 10}}};
  {
    FileUserPresetsDataSource testee{userPresetsPath()};
    testee.createPreset(presetToSave);
  }

  FileUserPresetsDataSource testee{userPresetsPath()};
  ASSERT_TRUE(testee.contains("user-preset-1"));
  const auto presets = testee.presets();

  ASSERT_EQ(1u, presets.size());
  const auto& savedPreset = presets.front();
  EXPECT_EQ(presetToSave.id(), savedPreset.id());
  EXPECT_EQ(presetToSave.name(), savedPreset.name());
  EXPECT_FALSE(savedPreset.isFactory());

  EXPECT_EQ(presetToSave.parameters(), savedPreset.parameters());
}

TEST(FileUserPresetsDataSource, DeletesExistingPresetFromDisk) {
  const wolfsound::WhenLeavingScopeExecute cleanup{[&] {
    std::for_each(std::filesystem::directory_iterator{userPresetsPath()},
                  std::filesystem::directory_iterator{}, [](const auto& entry) {
                    if (entry.path().extension() == ".json") {
                      EXPECT_TRUE(false);  // failure to remove the preset file
                      std::filesystem::remove(entry.path());
                    }
                  });
  }};

  const PresetV2 presetToSave{PresetMetadata{
                                  .name = "User Preset 1",
                                  .isFactory = false,
                                  .id = "user-preset-1",
                              },
                              {{.id = "param1", .value = 10}}};
  {
    FileUserPresetsDataSource testee{userPresetsPath()};
    testee.createPreset(presetToSave);
  }
  FileUserPresetsDataSource testee{userPresetsPath()};
  EXPECT_TRUE(testee.contains("user-preset-1"));

  testee.deletePreset("User Preset 1");
}

TEST(FileUserPresetsDataSource, SanitizesFilename) {
  EXPECT_EQ("user_preset_1.json",
            FileUserPresetsDataSource::filenameFrom("User Preset 1"));
  EXPECT_EQ("user_preset%!$_2.json",
            FileUserPresetsDataSource::filenameFrom("User Preset%!@#$ 2"));
}

TEST(PresetSerialization, CanDeserializeJsonToPresetData) {
  const auto presetData = juce::FromVar::convert<PresetData>(juce::JSON::parse(
      juce::File{(factoryPresetsPath() / "min.json").string()}));
  ASSERT_TRUE(presetData.has_value());
  EXPECT_EQ("min-preset-id", presetData->metadata.id);
  EXPECT_EQ("Min (Factory Preset)", presetData->metadata.name);
  EXPECT_EQ(1, presetData->metadata.presetVersion);
  const auto& parameters = presetData->parameters;
  ASSERT_EQ(4u, parameters.size());
  EXPECT_EQ("floatParam", parameters[0].id);
  EXPECT_EQ(1.f, std::get<float>(parameters[0].value));
  EXPECT_EQ("boolParam", parameters[1].id);
  EXPECT_FALSE(get<bool>(parameters[1].value));
  EXPECT_EQ("intParam", parameters[2].id);
  EXPECT_EQ(5, get<int>(parameters[2].value));
  EXPECT_EQ("choiceParam", parameters[3].id);
  EXPECT_EQ("choice 0", get<std::string>(parameters[3].value));
}
}  // namespace eden::plugin