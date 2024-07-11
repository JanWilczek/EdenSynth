#include "gtest/gtest.h"
#include "ProductionPresetManager.h"
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include <future>

namespace eden_vst_test {
TEST(ProductionPresetManager, SaveAndLoadPreset) {
  // given an audio processor with a parameter
  EdenSynthAudioProcessor audioProcessor{};
  auto sampleParameter =
      audioProcessor.getPluginParameters().getRawParameterValue(
          "frequencyOfA4");
  *sampleParameter = 440.f;
  auto& presetManager = audioProcessor.getPresetManager();
  juce::MemoryBlock currentPresetData;
  audioProcessor.getStateInformation(currentPresetData);
  constexpr auto presetName = "TestPreset";
  ASSERT_TRUE(presetManager.saveOrOverwriteCurrentPreset(presetName));
  *sampleParameter = 450.f;

  // when we load a preset
  ASSERT_TRUE(presetManager.loadPreset(presetName));

  // then plugin parameters get overwritten
  ASSERT_FLOAT_EQ(440.f, *sampleParameter);
}

TEST(ProductionPresetManager, CannotOverwritePresetWithSave) {
  // given
  EdenSynthAudioProcessor audioProcessor{};
  auto& presetManager = audioProcessor.getPresetManager();
  presetManager.saveCurrentPreset("TestPreset");

  // when
  const auto result = presetManager.saveCurrentPreset("TestPreset");

  // then
  ASSERT_FALSE(result);
  ASSERT_EQ(eden_vst::PresetSavingError::PresetWithNameExists, result.error());
}

TEST(ProductionPresetManager, SavedPresetIsAccessible) {
  // given
  EdenSynthAudioProcessor audioProcessor{};
  auto& presetManager = audioProcessor.getPresetManager();

  // when
  const auto result = presetManager.saveCurrentPreset("TestPreset");

  // then
  const auto presets = presetManager.presets();
  ASSERT_TRUE(std::ranges::contains(presets, "TestPreset"));
}

TEST(ProductionPresetManager, CannotLoadNonexistingPreset) {
  // given
  EdenSynthAudioProcessor audioProcessor{};
  auto& presetManager = audioProcessor.getPresetManager();

  // when loading non-existing preset
  const auto result = presetManager.loadPreset("TestPreset");

  // then
  ASSERT_FALSE(result);
  ASSERT_EQ(eden_vst::PresetLoadingError::DoesNotExist, result.error());
}
}  // namespace eden_vst_test
