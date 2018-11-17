#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "GeneratorComponent.h"
#include "ModifierComponent.h"

class EdenSynthAudioProcessorEditor : public AudioProcessorEditor
{
public:
	EdenSynthAudioProcessorEditor(EdenSynthAudioProcessor&, AudioProcessorValueTreeState&, const eden_vst::WaveTablePathProvider&);
	~EdenSynthAudioProcessorEditor();

	void paint(Graphics&) override;
	void resized() override;

private:
	EdenSynthAudioProcessor& _processor;
	
	GeneratorComponent _generatorComponent;
	ModifierComponent _modifierComponent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EdenSynthAudioProcessorEditor)
};
