#include "PresetSaver.h"
#include "JuceHeader.h"

namespace eden_vst {
PresetSaver::PresetSaver(juce::AudioProcessorValueTreeState& vts)
    : _pluginParameters{vts} {}

void PresetSaver::operator()() {
  saveCurrentPreset();
}

void PresetSaver::saveCurrentPreset() {
  auto state = _pluginParameters.copyState();
  // TODO: Make unique_ptr
  std::shared_ptr<XmlElement> presetXML(state.createXml().release());

  if (not presetXML) {
    // TODO: Display warning.
    return;
  }

  _savePresetDialog = std::make_shared<juce::FileChooser>(
      "Save the preset to an XML file", File(), "*.xml");
  constexpr auto saveSingleFileAndWarnAboutOverwriting =
      FileBrowserComponent::canSelectFiles | FileBrowserComponent::saveMode |
      FileBrowserComponent::warnAboutOverwriting;

  _savePresetDialog->launchAsync(
      saveSingleFileAndWarnAboutOverwriting,
      [presetXML = std::move(presetXML)](const FileChooser& dialog) {
        File presetOutputPath(dialog.getResult());
        if (not presetXML->writeTo(presetOutputPath)) {
          // TODO: Display warning.
        }
      });
}
}  // namespace eden_vst
