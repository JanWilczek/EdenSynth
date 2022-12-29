#include "PresetSaver.h"
#include "FileHelper.h"
#include "JuceHeader.h"

namespace eden_vst {
PresetSaver::PresetSaver(juce::AudioProcessorValueTreeState& vts)
    : _pluginParameters{vts} {}

void PresetSaver::operator()() {
  saveCurrentPreset();
}

void PresetSaver::saveCurrentPreset() {
  auto state = _pluginParameters.copyState();
  // A closure binding to an std::function must be copy-constructible;
  // hence the shared_ptr instead of a (more natural) unique_ptr.
  std::shared_ptr<XmlElement> presetXML(state.createXml().release());

  if (not presetXML) {
    // TODO: Display warning.
    return;
  }

  constexpr auto CANCEL_BUTTON_ID = 0;
  constexpr auto SAVE_PRESET_BUTTON_ID = 1;
  constexpr auto TEXT_EDITOR_NAME = "presetName";
  _savePresetDialog = std::make_shared<AlertWindow>(
      "Save preset", "Give the name of the preset", MessageBoxIconType::NoIcon);
  _savePresetDialog->addTextEditor(TEXT_EDITOR_NAME, "DefaultPresetName");
  _savePresetDialog->addButton("Cancel", CANCEL_BUTTON_ID);
  _savePresetDialog->addButton("Save preset", SAVE_PRESET_BUTTON_ID);
  auto textEditor = _savePresetDialog->getTextEditor(TEXT_EDITOR_NAME);
  _savePresetDialog->enterModalState(
      true, ModalCallbackFunction::create([presetXML = std::move(presetXML),
                                           textEditor,
                                           dialog = _savePresetDialog.get()](
                                              int pressedButtonIndex) {
        dialog->exitModalState(pressedButtonIndex);
        dialog->setVisible(false);

        if (pressedButtonIndex != SAVE_PRESET_BUTTON_ID or not textEditor) {
          return;
        }

        const auto presetName = textEditor->getText();
        if (presetName.isEmpty()) {
          return;
        }

        const auto presetOutputPath = File(
            (FileHelper::assetsPath() / "presets" / presetName.toStdString())
                .replace_extension("xml")
                .native()
                .c_str());

        if (presetOutputPath.exists()) {
          // TODO: Display "overwrite?" dialog
        }

        if (not presetXML->writeTo(presetOutputPath)) {
          // TODO: Display warning.
        }

        // TODO: Call a callback that a preset has been added.
      }));
}
}  // namespace eden_vst
