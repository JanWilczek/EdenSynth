#include "gtest/gtest.h"
#include "ProductionPresetManager.h"
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include <future>

namespace eden::plugin_test {
class ProductionPresetManagerTest : public ::testing::Test {
  juce::ScopedJuceInitialiser_GUI _guiInitializer;

protected:
  EdenSynthAudioProcessor audioProcessor{};

  void SetUp() override {
    std::filesystem::create_directory(testPresetsPath());
  }

  void TearDown() override { std::filesystem::remove_all(testPresetsPath()); }

private:
  std::filesystem::path testPresetsPath() {
    static const auto path =
        File::getSpecialLocation(
            File::SpecialLocationType::currentExecutableFile)
            .getParentDirectory()
            .getChildFile("testPresets");
    return path.getFullPathName().toStdString();
  }
};

TEST_F(ProductionPresetManagerTest, CannotOverwritePresetWithSave) {
  // given
  auto& presetManager = audioProcessor.getPresetManager();
  [[maybe_unused]] const auto firstTimeResult =
      presetManager.saveCurrentPreset("TestPreset");

  // when
  const auto result = presetManager.saveCurrentPreset("TestPreset");

  // then
  ASSERT_FALSE(result);
  ASSERT_EQ(eden::plugin::PresetSavingError::PresetWithNameExists,
            result.error());
}

TEST_F(ProductionPresetManagerTest, SavedPresetIsAccessible) {
  // given
  auto& presetManager = audioProcessor.getPresetManager();

  // when
  const auto result = presetManager.saveCurrentPreset("TestPreset");
  ASSERT_TRUE(result.has_value());

  // then
  const auto presets = presetManager.presets();
  // ASSERT_TRUE(std::ranges::contains(presets, "TestPreset"));
}

TEST_F(ProductionPresetManagerTest, CannotLoadNonexistingPreset) {
  // given
  // auto& presetManager = audioProcessor.getPresetManager();

  // when loading non-existing preset
  // const auto result = presetManager.loadPreset("TestPreset");

  // then
  // ASSERT_FALSE(result);
  // ASSERT_EQ(eden::plugin::PresetLoadingError::DoesNotExist, result.error());
}

TEST_F(ProductionPresetManagerTest, CannotSavePresetWithEmptyName) {
  // when
  auto& presetManager = audioProcessor.getPresetManager();
  const auto result = presetManager.saveCurrentPreset("");

  // then
  ASSERT_FALSE(result);
  ASSERT_EQ(eden::plugin::PresetSavingError::InvalidPresetName, result.error());
}
}  // namespace eden::plugin_test
