/// 
/// \author Jan Wilczek
/// \date 03.09.2018
/// 
#include "synth/Voice.h"

#include "eden/AudioBuffer.h"
#include "eden/MidiMessage.h"
#include "synth/envelope/ADBDR.h"

namespace eden::synth
{
	Voice::Voice(double sampleRate)
		: _sampleRate(sampleRate)
		, _signalGenerator(std::make_unique<wavetable::SignalGenerator>(_sampleRate))
		, _subtractiveModule(std::make_unique<subtractive::SubtractiveModule>())
		, _waveshapingModule(std::make_unique<waveshaping::WaveshapingModule>())
		, _envelopeGenerator(std::make_unique<envelope::ADBDR>(_sampleRate, 100ms, 100ms, 10000ms, 1000ms, SampleType(0.8)))
	{
		_envelopeGenerator->setOnEnvelopeEndedCallback([this]() { finalizeVoice(); });
	}

	void Voice::startNote(int midiNoteNumber, float velocity)
	{
		_isActive = true;
		_currentNote = midiNoteNumber;
		_velocity = static_cast<SampleType>(velocity);

		const auto pitch = calculatePitch(_currentNote, 0);	// TODO: Handle pitch wheel.
		setPitch(pitch);

		_envelopeGenerator->keyOn();
	}

	void Voice::stopNote(float /* velocity */)
	{
		_envelopeGenerator->keyOff();
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

			applyVelocity(outputBuffer.getWritePointer(MONO_CHANNEL_TO_PROCESS), startSample, samplesToRender);

			//duplicateMonoChannel(outputBuffer, 0, startSample, samplesToRender);
		}
	}

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
		_envelopeGenerator->setSampleRate(_sampleRate);
	}

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

	void Voice::applyVelocity(SampleType* channel, int startSample, int samplesToRender)
	{
		for (int sample = startSample; sample < startSample + samplesToRender; ++sample)
		{
			channel[sample] *= _velocity;
		}
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
