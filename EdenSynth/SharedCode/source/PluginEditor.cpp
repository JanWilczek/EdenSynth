#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WaveTablePathProvider.h"

EdenSynthAudioProcessorEditor::EdenSynthAudioProcessorEditor(EdenSynthAudioProcessor& p, AudioProcessorValueTreeState& vts, const eden_vst::WaveTablePathProvider& pathProvider)
	: AudioProcessorEditor(&p)
	, _processor(p)
	, _generalSettingsComponent(vts)
	, _generatorComponent(vts, pathProvider)
	, _modifierComponent(vts)
{
	setSize(1000, 500);

	addAndMakeVisible(_generalSettingsComponent);
	addAndMakeVisible(_generatorComponent);
	addAndMakeVisible(_modifierComponent);
}

EdenSynthAudioProcessorEditor::~EdenSynthAudioProcessorEditor()
{
}

void EdenSynthAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void EdenSynthAudioProcessorEditor::resized()
{
	_generalSettingsComponent.setBounds(0, 0, 120, 500);
	_generatorComponent.setBounds(120, 0, 380, 500);
	_modifierComponent.setBounds(500, 0, 380, 500);
}
