#include <PluginProcessor.h>
#include <gtest/gtest.h>

namespace eden_vst {
TEST(PluginProcessor, SetStateInformationRestoresStateCorrectly) {
  juce::ScopedJuceInitialiser_GUI _guiInitializer;
  EdenSynthAudioProcessor audioProcessor{[](AudioProcessorValueTreeState&) {
    return std::unique_ptr<PresetManager>{};
  }};
  [[maybe_unused]] auto& parameters = audioProcessor.getPluginParameters();
}
}  // namespace eden_vst
