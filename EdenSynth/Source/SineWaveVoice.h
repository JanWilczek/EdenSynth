#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SineWaveSound.h"

class SineWaveVoice : public SynthesiserVoice
{
public:

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<SineWaveSound*>(sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
	{
		currentAngle = 0.0;
		level = velocity * 0.15;
		tailOff = 0.0;

		const auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		const auto cyclesPerSample = cyclesPerSecond / getSampleRate();

		angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		if (angleDelta != 0.0)
		{
			if (tailOff > 0.0)
			{
				while (--numSamples > 0)
				{
					generateSample(outputBuffer, startSample);

					tailOff *= 0.99;

					if (tailOff <= 0.005f)
					{
						clearCurrentNote();

						angleDelta = 0.0f;
						break;
					}
				}
			}
			else
			{
				while (--numSamples >= 0)
				{
					generateSample(outputBuffer, startSample);
				}
			}
		}
	}

	void stopNote(float, bool allowTailOff) override
	{
		if (allowTailOff)
		{
			if (tailOff == 0.0)
			{
				tailOff = 1.0;
			}
		}
		else
		{
			clearCurrentNote();
			angleDelta = 0.0f;
		}
	}

	void pitchWheelMoved(int newPitchWheelValue) override
	{
		
	}

	void controllerMoved(int controllerNumber, int newControllerValue) override
	{
		
	}

private:
	void generateSample(AudioBuffer<float>& outputBuffer, int& startSample)
	{
		auto currentSample = std::sin(currentAngle) * level;

		if (tailOff > 0.0)
		{
			currentSample *= tailOff;
		}

		for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
		{
			outputBuffer.addSample(i, startSample, currentSample);
		}

		currentAngle += angleDelta;
		++startSample;
	}

private:
	double currentAngle = 0.0;
	double angleDelta = 0.0;
	double level = 0.0;
	double tailOff = 0.0;
};
