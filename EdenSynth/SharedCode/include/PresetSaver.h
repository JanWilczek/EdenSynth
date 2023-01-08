#pragma once
#include <memory>
#include <functional>

namespace juce {
class AudioProcessorValueTreeState;
class AlertWindow;
}  // namespace juce

namespace eden_vst {
class PresetSaver {
public:
  explicit PresetSaver(juce::AudioProcessorValueTreeState&);
  void operator()(std::function<void()> onPresetAdded);

private:
  void saveCurrentPreset(std::function<void()> onPresetAdded);

  juce::AudioProcessorValueTreeState& _pluginParameters;
  // A closure binding to an std::function must be copy-constructible;
  // hence the shared_ptr instead of a (more natural) unique_ptr.
  std::shared_ptr<juce::AlertWindow> _savePresetDialog;
};
}  // namespace eden_vst
