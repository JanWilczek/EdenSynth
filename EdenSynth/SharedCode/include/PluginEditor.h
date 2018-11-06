#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class EdenSynthAudioProcessorEditor : public AudioProcessorEditor
{
public:
	EdenSynthAudioProcessorEditor(EdenSynthAudioProcessor&, AudioProcessorValueTreeState&);
	~EdenSynthAudioProcessorEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

private:
	void waveTableChanged();

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	EdenSynthAudioProcessor& _processor;
	Label _title{ "headLabel", "Eden Synthesiser" };
	Font _titleFont{ 12.0f };
	ComboBox _waveTables;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EdenSynthAudioProcessorEditor)
};
