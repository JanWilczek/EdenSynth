#include "gtest/gtest.h"
#include "ProductionPresetManager.h"
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include <future>

namespace eden_vst_test {
TEST(ProductionPresetManager, SaveAndLoadPreset) {
  EdenSynthAudioProcessor audioProcessor{};
  auto sampleParameter =
      audioProcessor.getPluginParameters().getRawParameterValue(
          "frequencyOfA4");
  *sampleParameter = 440.f;
  auto& presetManager = audioProcessor.getPresetManager();
  juce::MemoryBlock currentPresetData;
  audioProcessor.getStateInformation(currentPresetData);
  constexpr auto presetName = "TestPreset";
  presetManager.saveCurrentPreset(presetName);
  *sampleParameter = 450.f;
  presetManager.loadPreset(presetName);

  ASSERT_FLOAT_EQ(440.f, *sampleParameter);
}
}  // namespace eden_vst_test
