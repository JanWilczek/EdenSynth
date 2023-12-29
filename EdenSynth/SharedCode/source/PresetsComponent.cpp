#include "PresetsComponent.h"
#include "viewmodels/PresetsViewModel.h"

namespace {
constexpr auto SAVE_PRESET_BUTTON_ID = 1;
constexpr auto TEXT_EDITOR_NAME = "presetName";

std::unique_ptr<AlertWindow> makePresetNameInputDialog() {
  constexpr auto CANCEL_BUTTON_ID = 0;
  auto dialog = std::make_unique<AlertWindow>(
      "Save preset", "Give the name of the preset", MessageBoxIconType::NoIcon);
  dialog->addTextEditor(TEXT_EDITOR_NAME, "DefaultPresetName");
  dialog->addButton("Cancel", CANCEL_BUTTON_ID);
  dialog->addButton("Save preset", SAVE_PRESET_BUTTON_ID);
  auto textEditor = dialog->getTextEditor(TEXT_EDITOR_NAME);
  textEditor->setJustification(Justification::centred);
  return dialog;
}
}  // namespace

PresetsComponent::PresetsComponent(
    std::unique_ptr<eden_vst::viewmodels::PresetsViewModel> presetsViewModel)
    : _viewModel{std::move(presetsViewModel)} {
  _presetLabel.setJustificationType(Justification::right);
  addAndMakeVisible(_presetLabel);

  refreshPresetList();

  _preset.onChange = [this] {
    _viewModel->onSelectedPresetChanged(_preset.getSelectedId());
  };
  addAndMakeVisible(_preset);

  _savePresetButton.setButtonText("Save preset");
  _savePresetButton.onClick = [this] { _viewModel->onSavePresetClicked(); };
  addAndMakeVisible(_savePresetButton);

  _viewModel->setOnPresetListChangedListener([this] {
    refreshPresetList();
    _preset.setSelectedId(_viewModel->getDisplayedPresetId());
  });

  _viewModel->addErrorDialogListener(this);

  _viewModel->onPresetNameInputDialogVisibilityChanged(
      [this](eden_vst::Visibility visibility) {
        if (visibility == eden_vst::Visibility::Visible and
            _presetNameInputDialog == nullptr) {
          _presetNameInputDialog = makePresetNameInputDialog();
          _presetNameInputDialog->enterModalState(
              true,
              ModalCallbackFunction::create([this](int pressedButtonIndex) {
                if (pressedButtonIndex != SAVE_PRESET_BUTTON_ID) {
                  _viewModel->onPresetNameInputDialogCancelClicked();
                  return;
                }

                const auto textEditor =
                    _presetNameInputDialog->getTextEditor(TEXT_EDITOR_NAME);
                const auto presetName = textEditor->getText();

                _viewModel->onPresetNameGiven(presetName.toStdString());
              }));
        } else if (visibility == eden_vst::Visibility::Gone and
                   _presetNameInputDialog != nullptr) {
          _presetNameInputDialog->exitModalState(0);
          _presetNameInputDialog->setVisible(false);
          _presetNameInputDialog.reset(nullptr);
        }
      });

  _viewModel->onShouldOverridePresetDialogVisibilityChanged(
      [this](eden_vst::Visibility visibility,
             const std::string& presetToOverwriteName) {
        if (visibility == eden_vst::Visibility::Visible) {
          AlertWindow::showYesNoCancelBox(
              MessageBoxIconType::QuestionIcon, "Preset file exists",
              "A preset file with name " + presetToOverwriteName +
                  " already exists. Do you want to overwrite it?",
              "", "", "", nullptr,
              ModalCallbackFunction::create([this, presetToOverwriteName](
                                                int returnValue) {
                constexpr auto SHOULD_OVERWRITE_BUTTON_ID = 1;
                if (returnValue == SHOULD_OVERWRITE_BUTTON_ID) {
                  _viewModel->onOverwritePresetClicked(presetToOverwriteName);
                }
              }));
        }
      });
}

void PresetsComponent::refreshPresetList() {
  _preset.clear(dontSendNotification);

  for (const auto& [presetId, presetName] : _viewModel->getPresetList()) {
    _preset.addItem(presetName, presetId);
  }
}

void PresetsComponent::paint(juce::Graphics& g) {
  g.setColour(
      getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
  g.drawRect(getBounds(), 5);
}

void PresetsComponent::resized() {
  const auto PRESET_LABEL_WIDTH = getWidth() / 8;
  const auto PRESET_BOX_WIDTH = getWidth() / 2;
  const auto PRESET_BOX_HEIGHT = getHeight() / 2;
  const auto PRESET_BOX_X = (getWidth() - PRESET_BOX_WIDTH) / 2;
  const auto PRESET_BOX_Y = (getHeight() - PRESET_BOX_HEIGHT) / 2;
  const auto SAVE_PRESET_BUTTON_WIDTH = getWidth() / 8;
  const auto SAVE_PRESET_BUTTON_HEIGHT = PRESET_BOX_HEIGHT;
  const auto SAVE_PRESET_BUTTON_Y = PRESET_BOX_Y;
  constexpr auto COMPONENT_SPACING = 10;
  const auto PRESET_LABEL_X =
      PRESET_BOX_X - COMPONENT_SPACING - PRESET_LABEL_WIDTH;

  _presetLabel.setBounds(PRESET_LABEL_X, 0, PRESET_LABEL_WIDTH, getHeight());
  _preset.setBounds(PRESET_BOX_X, PRESET_BOX_Y, PRESET_BOX_WIDTH,
                    PRESET_BOX_HEIGHT);

  const auto SAVE_PRESET_BUTTON_X = _preset.getRight() + COMPONENT_SPACING;
  _savePresetButton.setBounds(SAVE_PRESET_BUTTON_X, SAVE_PRESET_BUTTON_Y,
                              SAVE_PRESET_BUTTON_WIDTH,
                              SAVE_PRESET_BUTTON_HEIGHT);
}

void PresetsComponent::showErrorDialogWithMessage(const std::string& message) {
  AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Error",
                                   message, "");
}
