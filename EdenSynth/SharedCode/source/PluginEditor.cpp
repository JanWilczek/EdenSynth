#include "PluginEditor.h"
#include "EdenAdapter.h"
#include "PluginProcessor.h"

namespace {
constexpr auto PRESETS_COMPONENT_HEIGHT = 50;
constexpr auto SYNTHESIZER_CONTROLS_COMPONENTS_HEIGHT = 500;
constexpr auto EDITOR_HEIGHT =
    PRESETS_COMPONENT_HEIGHT + SYNTHESIZER_CONTROLS_COMPONENTS_HEIGHT;
}  // namespace

EdenSynthAudioProcessorEditor::EdenSynthAudioProcessorEditor(
    EdenSynthAudioProcessor& p,
    AudioProcessorValueTreeState& vts,
    const eden_vst::EdenAdapter& adapter)
    : AudioProcessorEditor(&p),
      _processor(p),
      _generalSettingsComponent(vts),
      _generatorComponent(vts, adapter.getPathProvider()),
      _modifierComponent(vts, adapter),
      _outputSettingsComponent(vts) {
  setSize(1040, EDITOR_HEIGHT);

  addAndMakeVisible(_generalSettingsComponent);
  addAndMakeVisible(_generatorComponent);
  addAndMakeVisible(_modifierComponent);
  addAndMakeVisible(_outputSettingsComponent);
  addAndMakeVisible(_presetsComponent);
}

void EdenSynthAudioProcessorEditor::paint(Graphics& g) {
  g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void EdenSynthAudioProcessorEditor::resized() {
  _presetsComponent.setBounds(0, 0, getWidth(), PRESETS_COMPONENT_HEIGHT);

  _generalSettingsComponent.setBounds(
      0, _presetsComponent.getY() + _presetsComponent.getHeight(), 120,
      SYNTHESIZER_CONTROLS_COMPONENTS_HEIGHT);
  _generatorComponent.setBounds(
      _generalSettingsComponent.getX() + _generalSettingsComponent.getWidth(),
      _generalSettingsComponent.getY(), 420,
      SYNTHESIZER_CONTROLS_COMPONENTS_HEIGHT);
  _modifierComponent.setBounds(
      _generatorComponent.getX() + _generatorComponent.getWidth(),
      _generalSettingsComponent.getY(), 420,
      SYNTHESIZER_CONTROLS_COMPONENTS_HEIGHT);
  _outputSettingsComponent.setBounds(
      _modifierComponent.getX() + _modifierComponent.getWidth(),
      _modifierComponent.getY(),
      getWidth() - (_modifierComponent.getX() + _modifierComponent.getWidth()),
      SYNTHESIZER_CONTROLS_COMPONENTS_HEIGHT);
}
