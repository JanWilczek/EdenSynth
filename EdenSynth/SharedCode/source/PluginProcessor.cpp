#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "EdenAdapter.h"
#include <eden/AudioBuffer.h>
#include <eden/MidiBuffer.h>

#include <filesystem>
#include "utility/WaveFileReader.h"

//==============================================================================
EdenSynthAudioProcessor::EdenSynthAudioProcessor()
	:
#ifndef JucePlugin_PreferredChannelConfigurations
	AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
#endif
	_assetsPath(std::experimental::filesystem::current_path() / "assets")
	, _pluginParameters(*this, nullptr)
{
	eden_vst::EdenAdapter::addEdenParameters(_edenSynthesiser, _pluginParameters);
	_pluginParameters.state = ValueTree(Identifier("EdenSynthParameters"));
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

void EdenSynthAudioProcessor::setCurrentProgram(int /*index*/)
{
}

const String EdenSynthAudioProcessor::getProgramName(int /*index*/)
{
	return {};
}

void EdenSynthAudioProcessor::changeProgramName(int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void EdenSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	_edenSynthesiser.setSampleRate(sampleRate);
	_edenSynthesiser.setBlockLength(samplesPerBlock);
}

void EdenSynthAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EdenSynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
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

void EdenSynthAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;

	eden_vst::EdenAdapter::updateEdenParameters(_edenSynthesiser, _pluginParameters);

	eden::AudioBuffer edenAudioBuffer(buffer.getArrayOfWritePointers(), getTotalNumOutputChannels(), buffer.getNumSamples());
	eden::MidiBuffer edenMidiBuffer = eden_vst::EdenAdapter::convertToEdenMidi(midiMessages);

	_edenSynthesiser.processInputBlock(edenAudioBuffer, edenMidiBuffer);
}

//==============================================================================
bool EdenSynthAudioProcessor::hasEditor() const
{
	return true;
}

AudioProcessorEditor* EdenSynthAudioProcessor::createEditor()
{
	return new EdenSynthAudioProcessorEditor(*this, _pluginParameters);
}

//==============================================================================
void EdenSynthAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	auto state = _pluginParameters.copyState();
	const std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void EdenSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get())
	{
		if (xmlState->hasTagName(_pluginParameters.state.getType()))
		{
			_pluginParameters.replaceState(ValueTree::fromXml(*xmlState));
		}
	}
}

std::experimental::filesystem::path EdenSynthAudioProcessor::getAssetsPath() const
{
	return _assetsPath;
}

void EdenSynthAudioProcessor::setWaveTable(const std::string& filename)
{
	try
	{
		auto path = getAssetsPath() / "wavetables" / filename;

		eden::utility::WaveFileReader reader(path.string());
		const auto wave = reader.readSamples();
		_edenSynthesiser.setWaveTable(wave);
	}
	catch (...)
	{
		// TODO: Add error handling.
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new EdenSynthAudioProcessor();
}