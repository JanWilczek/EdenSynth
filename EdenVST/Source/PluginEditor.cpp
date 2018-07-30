/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EdenVstAudioProcessorEditor::EdenVstAudioProcessorEditor (EdenVstAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Set Editor's window's size
    setSize (200, 200);

	// Customize the slider
	midiVolume.setSliderStyle(Slider::LinearBarVertical);
	midiVolume.setRange(0.0, 127.0, 1.0);
	midiVolume.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	midiVolume.setPopupDisplayEnabled(true, false, this);
	midiVolume.setTextValueSuffix(" Volume");
	midiVolume.setValue(1.0);

	// Add necessary components to the Editor
	addAndMakeVisible(&midiVolume);

	// add the listener to the slider
	midiVolume.addListener(this);
}

EdenVstAudioProcessorEditor::~EdenVstAudioProcessorEditor()
{
}

//==============================================================================
void EdenVstAudioProcessorEditor::paint (Graphics& g)
{
    // fill the whole window white
    g.fillAll (Colours::white);

	// set the current drawing colour to black
    g.setColour (Colours::black);

	// set the font size and draw text to the screen
    g.setFont (15.0f);

    g.drawFittedText ("Midi Volume", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void EdenVstAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	
	// sets the position and size of the slider with arguments (x, y, width, height)
	midiVolume.setBounds(40, 30, 20, getHeight() - 60);
}

void EdenVstAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	processor.noteOnVel = midiVolume.getValue();
}
