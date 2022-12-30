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
    AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Error",
                                     "Failed to create the preset file.");
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
  if (not textEditor) {
    // Fatal error
    return;
  }
  textEditor->setJustification(Justification::centred);

  _savePresetDialog->enterModalState(
      true, ModalCallbackFunction::create([presetXML = std::move(presetXML),
                                           textEditor,
                                           dialog = _savePresetDialog.get()](
                                              int pressedButtonIndex) {
        dialog->exitModalState(pressedButtonIndex);
        dialog->setVisible(false);

        if (pressedButtonIndex != SAVE_PRESET_BUTTON_ID) {
          return;
        }

        const auto presetName = textEditor->getText();
        if (presetName.isEmpty()) {
          return;
        }

        const auto presetOutputPath =
            (FileHelper::presetsPath() / presetName.toStdString())
                .replace_extension("xml");

        if (std::filesystem::exists(presetOutputPath)) {
          if (AlertWindow::showYesNoCancelBox(
                  MessageBoxIconType::QuestionIcon, "Preset file exists",
                  "A preset file with the given name already exists. Do you "
                  "want to overwrite it?",
                  "", "", "", nullptr, nullptr) == 1) {
            // TODO: Call a callback that the user does not want to overwrite.
            return;
          }
        }

        const auto presetFile = File(presetOutputPath.c_str());
        if (not presetXML->writeTo(presetFile)) {
          AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon,
                                           "Error",
                                           "Failed to save the preset file.");
        }

        // TODO: Call a callback that a preset has been added.
      }));
}
}  // namespace eden_vst
