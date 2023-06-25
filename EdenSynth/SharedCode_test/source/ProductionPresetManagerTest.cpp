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
  auto presetManager = audioProcessor.getPresetManager();
  juce::MemoryBlock currentPresetData;
  audioProcessor.getStateInformation(currentPresetData);
  std::promise<std::string> savedPresetNamePromise;
  auto savedPresetNameFuture = savedPresetNamePromise.get_future();
  presetManager.saveCurrentPreset([promise = std::move(savedPresetNamePromise)](
                                      const std::string& presetName) mutable {
    // get the saved preset name
    promise.set_value(presetName);
  });
  *sampleParameter = 450.f;
  const auto savedPresetName = savedPresetNameFuture.get();
  presetManager.loadPreset(savedPresetName);

  ASSERT_FLOAT_EQ(440.f, *sampleParameter);
}
}  // namespace eden_vst_test
