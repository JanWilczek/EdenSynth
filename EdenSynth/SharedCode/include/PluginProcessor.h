#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <eden/EdenSynthesiser.h>
#include <filesystem>

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
	std::experimental::filesystem::path getAssetsPath() const;
	void setWaveTable(const std::string& filename);

private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EdenSynthAudioProcessor)

		std::experimental::filesystem::path _assetsPath;
	eden::EdenSynthesiser _edenSynthesiser;
	AudioProcessorValueTreeState _pluginParameters;
};