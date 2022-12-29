#pragma once
#include <memory>

namespace juce {
class AudioProcessorValueTreeState;
class FileChooser;
}  // namespace juce

namespace eden_vst {
class PresetSaver {
public:
  explicit PresetSaver(juce::AudioProcessorValueTreeState&);
  void operator()();

private:
  void saveCurrentPreset();

  juce::AudioProcessorValueTreeState& _pluginParameters;
  // A closure binding to an std::function must be copy-constructible;
  // hence the shared_ptr instead of a (more natural) unique_ptr.
  std::shared_ptr<juce::FileChooser> _savePresetDialog;
};
}  // namespace eden_vst
