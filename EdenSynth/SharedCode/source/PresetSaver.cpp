#include "PresetSaver.h"
#include "FileHelper.h"
#include "JuceHeader.h"

namespace eden_vst {
namespace {
constexpr auto SAVE_PRESET_BUTTON_ID = 1;
constexpr auto TEXT_EDITOR_NAME = "presetName";

std::shared_ptr<AlertWindow> makeSavePresetDialog() {
  constexpr auto CANCEL_BUTTON_ID = 0;
  auto dialog = std::make_shared<AlertWindow>(
      "Save preset", "Give the name of the preset", MessageBoxIconType::NoIcon);
  dialog->addTextEditor(TEXT_EDITOR_NAME, "DefaultPresetName");
  dialog->addButton("Cancel", CANCEL_BUTTON_ID);
  dialog->addButton("Save preset", SAVE_PRESET_BUTTON_ID);
  auto textEditor = dialog->getTextEditor(TEXT_EDITOR_NAME);
  textEditor->setJustification(Justification::centred);
  return dialog;
}
}  // namespace

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

  _savePresetDialog = makeSavePresetDialog();
  _savePresetDialog->enterModalState(
      true, ModalCallbackFunction::create([dialog = _savePresetDialog.get(),
                                           presetXML = std::move(presetXML)](
                                              int pressedButtonIndex) {
        dialog->exitModalState(pressedButtonIndex);
        dialog->setVisible(false);

        if (pressedButtonIndex != SAVE_PRESET_BUTTON_ID) {
          return;
        }

        const auto textEditor = dialog->getTextEditor(TEXT_EDITOR_NAME);
        const auto presetName = textEditor->getText();
        const auto presetOutputPath =
            (FileHelper::presetsPath() / presetName.toStdString())
                .replace_extension("xml");

        auto saveAction = [presetXML = std::move(presetXML),
                           presetOutputPath = presetOutputPath]() {
          if (presetOutputPath.empty() or not presetOutputPath.has_filename() or
              not presetXML) {
            return;
          }

          const auto presetFile = File(presetOutputPath.c_str());
          if (not presetXML->writeTo(presetFile)) {
            AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon,
                                             "Error",
                                             "Failed to save the preset file.");
          }
        };

        if (std::filesystem::exists(presetOutputPath)) {
          AlertWindow::showYesNoCancelBox(
              MessageBoxIconType::QuestionIcon, "Preset file exists",
              "A preset file with the given name already exists. Do you "
              "want to overwrite it?",
              "", "", "", nullptr,
              ModalCallbackFunction::create(
                  [saveAction = std::move(saveAction)](int returnValue) {
                    constexpr auto SHOULD_OVERWRITE_BUTTON_ID = 1;
                    if (returnValue == SHOULD_OVERWRITE_BUTTON_ID) {
                      saveAction();
                    }
                  }));
          return;
        }

        saveAction();

        // TODO: Call a callback that a preset has been added.
      }));
}
}  // namespace eden_vst
