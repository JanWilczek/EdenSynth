#pragma once
#include <filesystem>
#include <memory>
#include "JuceHeader.h"

namespace eden_vst {
class OverwriteCallback;

struct SaveAction {
  void operator()();
  std::shared_ptr<juce::XmlElement> presetXML;
  std::filesystem::path presetOutputPath;
};

class PresetSaver {
public:
  explicit PresetSaver(juce::AudioProcessorValueTreeState&);
  void operator()();

private:
  void saveCurrentPreset();

  juce::AudioProcessorValueTreeState& _pluginParameters;
  // A closure binding to an std::function must be copy-constructible;
  // hence the shared_ptr instead of a (more natural) unique_ptr.
  // std::shared_ptr<juce::FileChooser> _savePresetDialog;
  std::shared_ptr<juce::AlertWindow> _savePresetDialog;

  SaveAction _saveAction;

  class OverwriteCallback : public juce::ModalComponentManager::Callback {
  public:
    explicit OverwriteCallback(SaveAction& saveAction);
    void modalStateFinished(int returnValue) override;

  private:
    SaveAction& _saveAction;
  };
  // A closure binding to an std::function must be copy-constructible;
  // hence the shared_ptr instead of a (more natural) unique_ptr.
  std::shared_ptr<OverwriteCallback> _callback;
};
}  // namespace eden_vst
