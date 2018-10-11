/// 
/// \author Jan Wilczek
/// \date 03.09.2018
/// 
#include "synth/Voice.h"

#include "eden/AudioBuffer.h"
#include "eden/MidiMessage.h"

namespace eden::synth
{
	Voice::Voice(double sampleRate)
		: _sampleRate(sampleRate)
		, _signalGenerator(std::make_unique<wavetable::SignalGenerator>(_sampleRate))
		, _subtractiveModule(std::make_unique<subtractive::SubtractiveModule>())
		, _waveshapingModule(std::make_unique<waveshaping::WaveshapingModule>())
		, _envelopeGenerator(std::make_unique<envelope::Envelope>())
	{
	}

	//void Voice::startNote(int midiNoteNumber, float velocity, int currentPitchWheelPosition)
	//{
	//	_currentNote = midiNoteNumber;
	//	currentAngle = 0.0;
	//	level = static_cast<AudioBuffer::SampleType>(velocity * 0.15);
	//	tailOff = 0.0;

	//	const auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	//	const auto cyclesPerSample = cyclesPerSecond / getSampleRate();

	//	angleDelta = static_cast<AudioBuffer::SampleType>(cyclesPerSample * 2.0 * 3.141);//juce::MathConstants<double>::pi;
	//}

	void Voice::startNote(int midiNoteNumber, float velocity)
	{
		_isActive = true;
		_currentNote = midiNoteNumber;
		_velocity = static_cast<SampleType>(velocity);

		const auto pitch = calculatePitch(_currentNote, 0);	// TODO: Handle pitch wheel.
		setPitch(pitch);

		_envelopeGenerator->keyOn();
	}

	void Voice::renderBlock(AudioBuffer& outputBuffer, int startSample, int samplesToRender)
	{
		if (isPlaying())
		{
			constexpr auto MONO_CHANNEL_TO_PROCESS = 0;

			_signalGenerator->generateSignal(outputBuffer.getWritePointer(MONO_CHANNEL_TO_PROCESS), startSample, samplesToRender);

			_subtractiveModule->process(outputBuffer, startSample, samplesToRender);

			_waveshapingModule->process(outputBuffer, startSample, samplesToRender);

			_envelopeGenerator->apply(outputBuffer.getWritePointer(MONO_CHANNEL_TO_PROCESS), startSample, samplesToRender);

			applyVelocity(outputBuffer, startSample, samplesToRender);

			duplicateMonoChannel(outputBuffer, 0, startSample, samplesToRender);
		}
	}

	//void Voice::renderBlock(AudioBuffer& outputBuffer, int startSample, int numSamples)
	//{
	//	if (angleDelta != 0.0)
	//	{
	//		if (tailOff > 0.0)
	//		{
	//			while (--numSamples > 0)
	//			{
	//				generateSample(outputBuffer, startSample);

	//				tailOff *= static_cast<AudioBuffer::SampleType>(0.998);

	//				if (tailOff <= 0.005f)
	//				{
	//					//clearCurrentNote();
	//					currentAngle = 0.0;
	//					angleDelta = 0.0f;
	//					tailOff = 0.0f;
	//					_currentNote = -1;
	//					break;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			while (--numSamples >= 0)
	//			{
	//				generateSample(outputBuffer, startSample);
	//			}
	//		}
	//	}
	//}

	void Voice::stopNote(float /* velocity */)
	{
		_envelopeGenerator->keyOff();
	}

	//void Voice::stopNote(float, bool allowTailOff)
	//{
	//	if (allowTailOff)
	//	{
	//		if (tailOff == 0.0)
	//		{
	//			tailOff = 1.0;
	//		}
	//	}
	//	else
	//	{
	//		//clearCurrentNote();
	//		angleDelta = 0.0f;
	//		_currentNote = -1;
	//		currentAngle = 0.0;
	//	}
	//}

	void Voice::pitchWheelMoved(int newPitchWheelValue)
	{
		const auto newPitch = calculatePitch(_currentNote, newPitchWheelValue);
		setPitch(newPitch);
	}

	//void controllerMoved(int controllerNumber, int newControllerValue) override
	//{

	//}*/

	bool Voice::isPlaying() const noexcept
	{
		return _isActive;
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
		_signalGenerator->setSampleRate(_sampleRate);
	}

	//void Voice::generateSample(AudioBuffer& outputBuffer, int& startSample)
	//{
	//	auto currentSample = static_cast<AudioBuffer::SampleType>(std::sin(currentAngle) * level);

	//	if (tailOff > 0.0)
	//	{
	//		currentSample *= tailOff;
	//	}

	//	for (auto i = outputBuffer.getNumChannels() - 1; i >= 0; --i)
	//	{
	//		outputBuffer.addSample(i, startSample, currentSample);
	//	}

	//	currentAngle += angleDelta;
	//	++startSample;
	//}

	void Voice::finalizeVoice()
	{
		_signalGenerator->stop();
		_currentNote = -1;
		_isActive = false;
	}

	double Voice::calculatePitch(int midiNoteNumber, int pitchWheelPosition)
	{
		return MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	}

	void Voice::setPitch(double newPitch)
	{
		_signalGenerator->setPitch(newPitch);

		_subtractiveModule->setPitch(newPitch);
	}

	void Voice::applyVelocity(AudioBuffer& outputBuffer, int startSample, int samplesToRender)
	{
		outputBuffer.forEachChannel([&](SampleType* channel)
		{
			for (int sample = startSample; sample < startSample + samplesToRender; ++sample)
			{
				channel[sample] *= _velocity;
			}
		});
	}

	void Voice::duplicateMonoChannel(AudioBuffer& outputBuffer, int channelToDuplicate, int startSample, int samplesToCopy)
	{
		for (auto channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
		{
			if (channel != channelToDuplicate)
			{
				std::copy(outputBuffer.getReadPointer(channelToDuplicate) + startSample,
					outputBuffer.getReadPointer(channelToDuplicate) + startSample + samplesToCopy,
					outputBuffer.getWritePointer(channel));
			}
		}
	}
}
