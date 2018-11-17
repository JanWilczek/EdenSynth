#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WaveTablePathProvider.h"

EdenSynthAudioProcessorEditor::EdenSynthAudioProcessorEditor(EdenSynthAudioProcessor& p, AudioProcessorValueTreeState& vts, const eden_vst::WaveTablePathProvider& pathProvider)
	: AudioProcessorEditor(&p)
	, _processor(p)
	, _generatorComponent(vts, pathProvider)
	, _modifierComponent(vts)
{
	setSize(1000, 500);

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
	_generatorComponent.setBounds(120, 0, 380, 500);
	_modifierComponent.setBounds(500, 0, 380, 500);
}
