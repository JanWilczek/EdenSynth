#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "EdenAdapter.h"

EdenSynthAudioProcessorEditor::EdenSynthAudioProcessorEditor(EdenSynthAudioProcessor& p, AudioProcessorValueTreeState& vts, const eden_vst::EdenAdapter& adapter)
	: AudioProcessorEditor(&p)
	, _processor(p)
	, _generalSettingsComponent(vts)
	, _generatorComponent(vts, adapter.getPathProvider())
	, _modifierComponent(vts, adapter)
	, _outputSettingsComponent(vts)
{
	setSize(1040, 500);

	addAndMakeVisible(_generalSettingsComponent);
	addAndMakeVisible(_generatorComponent);
	addAndMakeVisible(_modifierComponent);
	addAndMakeVisible(_outputSettingsComponent);
}

void EdenSynthAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void EdenSynthAudioProcessorEditor::resized()
{
	_generalSettingsComponent.setBounds(0, 0, 120, 500);
	_generatorComponent.setBounds(_generalSettingsComponent.getX() + _generalSettingsComponent.getWidth(), 0, 420, 500);
	_modifierComponent.setBounds(_generatorComponent.getX() + _generatorComponent.getWidth(), 0, 420, 500);
	_outputSettingsComponent.setBounds(_modifierComponent.getX() + _modifierComponent.getWidth(), _modifierComponent.getY(), getWidth() - (_modifierComponent.getX() + _modifierComponent.getWidth()), _modifierComponent.getHeight());
}
