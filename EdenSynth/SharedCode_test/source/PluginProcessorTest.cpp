#include <PluginProcessor.h>
#include <gtest/gtest.h>

namespace eden::plugin {
TEST(PluginProcessor, SetStateInformationRestoresStateCorrectly) {
  juce::ScopedJuceInitialiser_GUI _guiInitializer;
  EdenSynthAudioProcessor audioProcessor{};
  [[maybe_unused]] auto& parameters = audioProcessor.getPluginParameters();
  // TODO
}
}  // namespace eden::plugin
