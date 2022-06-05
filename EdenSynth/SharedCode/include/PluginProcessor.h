#pragma once
#include <filesystem>
#include <eden/EdenSynthesiser.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "EdenAdapter.h"
#include "WaveTablePathProvider.h"

class EdenSynthAudioProcessor : public AudioProcessor
{
public:
	//==============================================================================
	EdenSynthAudioProcessor();
	~EdenSynthAudioProcessor();

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

	//==============================================================================
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	//==============================================================================
	//void setWaveTable(const std::string& filename);

private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EdenSynthAudioProcessor)

	std::filesystem::path _assetsPath;
	eden::EdenSynthesiser _edenSynthesiser;
	eden_vst::EdenAdapter _edenAdapter;
	AudioProcessorValueTreeState _pluginParameters;
};
