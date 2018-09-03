#include <cmath>

#include "synth/Voice.h"

#include "eden/AudioBuffer.h"
#include "eden/MidiMessage.h"

namespace eden::synth
{
	void Voice::startNote(int midiNoteNumber, float velocity, int currentPitchWheelPosition)
	{
		_currentNote = midiNoteNumber;
		currentAngle = 0.0;
		level = velocity * 0.15;
		tailOff = 0.0;

		const auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		const auto cyclesPerSample = cyclesPerSecond / getSampleRate();

		angleDelta = cyclesPerSample * 2.0 * 3.141;//juce::MathConstants<double>::pi;
	}

	void Voice::renderBlock(AudioBuffer& outputBuffer, int startSample, int numSamples)
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
						//clearCurrentNote();

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

	void Voice::stopNote(float, bool allowTailOff)
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
			//clearCurrentNote();
			angleDelta = 0.0f;
		}
		_currentNote = -1;
	}

	/*void pitchWheelMoved(int newPitchWheelValue)
	{

	}

	void controllerMoved(int controllerNumber, int newControllerValue) override
	{

	}*/

	bool Voice::isPlaying() const noexcept
	{
		return _currentNote != -1;
	}

	bool Voice::isPlayingNote(const int midiNoteNumber) const noexcept
	{
		return _currentNote == midiNoteNumber;
	}
	
	double Voice::getSampleRate() const noexcept
	{
		return _sampleRate;
	}

	void Voice::setSampleRate(double newSampleRate)
	{
		_sampleRate = newSampleRate;
	}

	void Voice::generateSample(AudioBuffer& outputBuffer, int& startSample)
	{
		auto currentSample = static_cast<AudioBuffer::SampleType>(std::sin(currentAngle) * level);

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
}
