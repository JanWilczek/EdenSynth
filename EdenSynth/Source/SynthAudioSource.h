#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SineWaveSound.h"
#include "SineWaveVoice.h"

class SynthAudioSource : public AudioSource
{
public:
	SynthAudioSource()
	{
		for (auto i = 0; i < 4 ; i++)
		{
			synth.addVoice(new SineWaveVoice());
		}

		synth.addSound(new SineWaveSound());
	}

	void setUsingSineWaveSound()
	{
		synth.clearSounds();
	}

	void prepareToPlay(int, double sampleRate) override
	{
		synth.setCurrentPlaybackSampleRate(sampleRate);
	}

	void releaseResources() override {}

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
	{
		synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	}

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		
	}

private:
	Synthesiser synth;
};
