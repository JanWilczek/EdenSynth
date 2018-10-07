/// 
/// \author Jan Wilczek
/// \date 03.09.2018
/// 
#include <cmath>

#include "synth/Voice.h"

#include "eden/AudioBuffer.h"
#include "eden/MidiMessage.h"

namespace eden::synth
{
	Voice::Voice(double sampleRate)
		: _sampleRate(sampleRate)
	{
	}

	void Voice::startNote(int midiNoteNumber, float velocity, int currentPitchWheelPosition)
	{
		_currentNote = midiNoteNumber;
		currentAngle = 0.0;
		level = static_cast<AudioBuffer::SampleType>(velocity * 0.15);
		tailOff = 0.0;

		const auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		const auto cyclesPerSample = cyclesPerSecond / getSampleRate();

		angleDelta = static_cast<AudioBuffer::SampleType>(cyclesPerSample * 2.0 * 3.141);//juce::MathConstants<double>::pi;
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

					tailOff *= static_cast<AudioBuffer::SampleType>(0.998);

					if (tailOff <= 0.005f)
					{
						//clearCurrentNote();
						currentAngle = 0.0;
						angleDelta = 0.0f;
						tailOff = 0.0f;
						_currentNote = -1;
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
			_currentNote = -1;
			currentAngle = 0.0;
		}
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

		for (auto i = outputBuffer.getNumChannels() - 1; i >= 0; --i)
		{
			outputBuffer.addSample(i, startSample, currentSample);
		}

		currentAngle += angleDelta;
		++startSample;
	}
}
