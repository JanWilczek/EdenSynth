#include <vector>
#include <ranges>
#include <PluginProcessor.h>
#include <gtest/gtest.h>

namespace eden::plugin {
TEST(PluginProcessor, SetStateInformationRestoresStateCorrectly) {
  const juce::ScopedJuceInitialiser_GUI guiInitializer;
  std::unique_ptr<juce::AudioProcessor> audioProcessor{createPluginFilter()};
  const auto& parameters = audioProcessor->getParameters();

  // set parameters to random values and save them
  juce::Random random{42};
  std::vector<float> parameterValues;
  parameterValues.reserve(static_cast<size_t>(parameters.size()));
  for (auto* parameter : parameters) {
    ASSERT_FALSE(parameter->getName(100).isEmpty());
    parameter->setValue(random.nextFloat());
    parameterValues.push_back(parameter->getValue());
  }

  // serialize state
  juce::MemoryBlock serializedState;
  audioProcessor->getStateInformation(serializedState);

  // create a new plugin instance
  std::unique_ptr<juce::AudioProcessor> newAudioProcessor{createPluginFilter()};

  // deserialize state
  newAudioProcessor->setStateInformation(
      serializedState.getData(), static_cast<int>(serializedState.getSize()));

  // ensure that parameters have equal values
  ASSERT_EQ(parameters.size(), newAudioProcessor->getParameters().size());
  for (const auto& [parameter, value] :
       std::views::zip(newAudioProcessor->getParameters(), parameterValues)) {
    EXPECT_FLOAT_EQ(parameter->getValue(), value) << parameter->getName(100);
  }
}
}  // namespace eden::plugin
