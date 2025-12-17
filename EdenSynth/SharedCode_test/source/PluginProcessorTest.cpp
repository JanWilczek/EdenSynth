#include <PluginProcessor.h>
#include <gtest/gtest.h>

namespace eden_vst {
TEST(PluginProcessor, SetStateInformationRestoresStateCorrectly) {
  juce::ScopedJuceInitialiser_GUI _guiInitializer;
  EdenSynthAudioProcessor audioProcessor{};
  [[maybe_unused]] auto& parameters = audioProcessor.getPluginParameters();
  // TODO
}
}  // namespace eden_vst
