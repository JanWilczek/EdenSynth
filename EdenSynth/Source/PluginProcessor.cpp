/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "EdenAdapter.h"
#include <eden/AudioBuffer.h>
#include <eden/MidiBuffer.h>

#include <filesystem>
#include "utility/WaveFileReader.h"

//==============================================================================
EdenSynthAudioProcessor::EdenSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	auto path = std::experimental::filesystem::current_path();
	path = path / "assets" / "wavetables" / "AnalogPulse50Cycle.wav";

	eden::utility::WaveFileReader reader(path.string());
	const auto squareWave = reader.readSamples();
	_edenSynthesiser.setWaveTable(squareWave);
}

EdenSynthAudioProcessor::~EdenSynthAudioProcessor()
{
}

//==============================================================================
const String EdenSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EdenSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EdenSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EdenSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EdenSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EdenSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EdenSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EdenSynthAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String EdenSynthAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void EdenSynthAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void EdenSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	_edenSynthesiser.setSampleRate(sampleRate);
	_edenSynthesiser.setBlockLength(samplesPerBlock);
}

void EdenSynthAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EdenSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void EdenSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    auto totalNumOutputChannels = getTotalNumOutputChannels();

	eden::AudioBuffer edenAudioBuffer(buffer.getArrayOfWritePointers(), totalNumOutputChannels, buffer.getNumSamples());
	eden::MidiBuffer edenMidiBuffer;
	eden_vst::EdenAdapter::convertToEdenMidi(midiMessages, edenMidiBuffer);
	_edenSynthesiser.processInputBlock(edenAudioBuffer, edenMidiBuffer);
}

//==============================================================================
bool EdenSynthAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* EdenSynthAudioProcessor::createEditor()
{
    return new EdenSynthAudioProcessorEditor (*this);
}

//==============================================================================
void EdenSynthAudioProcessor::getStateInformation (MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EdenSynthAudioProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EdenSynthAudioProcessor();
}
