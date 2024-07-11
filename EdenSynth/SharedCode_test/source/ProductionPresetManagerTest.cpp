#include "gtest/gtest.h"
#include "ProductionPresetManager.h"
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include <future>

namespace eden_vst_test {
class ProductionPresetManagerTest : public ::testing::Test {
protected:
  EdenSynthAudioProcessor audioProcessor{
      [this](AudioProcessorValueTreeState& pluginParameters) {
        return std::make_unique<eden_vst::ProductionPresetManager>(
            testPresetsPath(), pluginParameters);
      }};

private:
  std::filesystem::path testPresetsPath() {
    const auto path = File::getSpecialLocation(
                          File::SpecialLocationType::currentExecutableFile)
                          .getParentDirectory()
                          .getChildFile("testPresets");
    // TODO: Move to SetUp and TearDown
    path.createDirectory();

    return path.getFullPathName().toStdString();
  }
};

TEST_F(ProductionPresetManagerTest, SaveAndLoadPreset) {
  // given an audio processor with a parameter
  auto sampleParameter =
      audioProcessor.getPluginParameters().getRawParameterValue(
          "frequencyOfA4");
  *sampleParameter = 440.f;
  auto& presetManager = audioProcessor.getPresetManager();

  constexpr auto presetName = "TestPreset";
  ASSERT_TRUE(presetManager.saveOrOverwriteCurrentPreset(presetName));
  *sampleParameter = 450.f;

  // when we load a preset
  ASSERT_TRUE(presetManager.loadPreset(presetName));

  // then plugin parameters get overwritten
  ASSERT_FLOAT_EQ(440.f, *sampleParameter);
}

TEST_F(ProductionPresetManagerTest, CannotOverwritePresetWithSave) {
  // given
  auto& presetManager = audioProcessor.getPresetManager();
  presetManager.saveCurrentPreset("TestPreset");

  // when
  const auto result = presetManager.saveCurrentPreset("TestPreset");

  // then
  ASSERT_FALSE(result);
  ASSERT_EQ(eden_vst::PresetSavingError::PresetWithNameExists, result.error());
}

TEST_F(ProductionPresetManagerTest, SavedPresetIsAccessible) {
  // given
  auto& presetManager = audioProcessor.getPresetManager();

  // when
  const auto result = presetManager.saveCurrentPreset("TestPreset");

  // then
  const auto presets = presetManager.presets();
  ASSERT_TRUE(std::ranges::contains(presets, "TestPreset"));
}

TEST_F(ProductionPresetManagerTest, CannotLoadNonexistingPreset) {
  // given
  auto& presetManager = audioProcessor.getPresetManager();

  // when loading non-existing preset
  const auto result = presetManager.loadPreset("TestPreset");

  // then
  ASSERT_FALSE(result);
  ASSERT_EQ(eden_vst::PresetLoadingError::DoesNotExist, result.error());
}
}  // namespace eden_vst_test
