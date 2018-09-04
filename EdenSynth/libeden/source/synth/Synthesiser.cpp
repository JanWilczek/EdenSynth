#include "synth/Synthesiser.h"

#include "eden/AudioBuffer.h"
#include "eden/MidiBuffer.h"
#include "eden/MidiMessage.h"
#include "synth/Voice.h"

namespace eden::synth
{
	Synthesiser::Synthesiser()
	{
		addVoices(16);
	}

	void Synthesiser::processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int numSamples)
	{
		processBlock(bufferToFill, midiBuffer, 0, numSamples);
	}

	void Synthesiser::processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int startSample, int numSamples)
	{
		auto midiIterator = midiBuffer.begin();

		while (numSamples > 0)
		{
			// 1. Proceed with voice rendering if there are no midi events.
			if (midiIterator == midiBuffer.end())
			{
				renderVoices(bufferToFill, startSample, numSamples);

				return;
			}

			const int samplesToNextMidiMessage = midiIterator->sampleTimeStamp - startSample;

			// 2. When message happened outside the rendered block.
			if (samplesToNextMidiMessage >= numSamples)
			{
				renderVoices(bufferToFill, startSample, numSamples);

				handleMidiMessage(midiIterator->message);

				break;
			}

			// 3. Render samples till the next midi message.
			renderVoices(bufferToFill, startSample, samplesToNextMidiMessage);

			handleMidiMessage(midiIterator->message);

			// 4. Advance in buffers.
			startSample += samplesToNextMidiMessage;
			numSamples -= samplesToNextMidiMessage;
			++midiIterator;
		}

		while (midiIterator != midiBuffer.end())
		{
			handleMidiMessage(midiIterator->message);
		}

		midiBuffer.clear();
	}

	double Synthesiser::getSampleRate() const noexcept
	{
		return _sampleRate;
	}

	void Synthesiser::setSampleRate(double newSampleRate)
	{
		_sampleRate = newSampleRate;
		for (auto voice : _voices)
		{
			voice->setSampleRate(newSampleRate);
		}
	}

	void Synthesiser::handleMidiMessage(MidiMessage& midiMessage)
	{
		const int channel = midiMessage.getChannel();
		const MidiMessage::MidiMessageType messageType = midiMessage.getType();

		switch(messageType)
		{
		case MidiMessage::MidiMessageType::NoteOn:
			noteOn(channel, midiMessage.getNoteNumber(), midiMessage.getVelocity());
			break;
		case MidiMessage::MidiMessageType::NoteOff:
			noteOff(channel, midiMessage.getNoteNumber(), midiMessage.getVelocity());
			break;
		default:
			break;
		}
	}

	void Synthesiser::renderVoices(AudioBuffer& outputBuffer, int startSample, int samplesToProcess)
	{
		for (auto voice : _voices)
		{
			voice->renderBlock(outputBuffer, startSample, samplesToProcess);
		}
	}

	void Synthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
	{
		// TODO: Check if already playing that note
		auto voice = getFreeVoice();
		if (voice)
		{
			voice->startNote(midiNoteNumber, velocity, 0);	// TODO: handle pitch wheel
		}

	}
	
	void Synthesiser::noteOff(const int midiChannel, const int midiNoteNumber, const float velocity)
	{
		auto voice = getVoicePlayingNote(midiNoteNumber);
		if (voice)
		{
			voice->stopNote(velocity, true);
		}
	}

	void Synthesiser::addVoices(unsigned numVoicesToAdd)
	{
		for (unsigned i = 0; i < numVoicesToAdd; ++i)
		{
			auto newVoice = std::make_shared<Voice>();
			newVoice->setSampleRate(_sampleRate);
			_voices.push_back(newVoice);
		}
	}

	std::shared_ptr<Voice> Synthesiser::getFreeVoice()
	{
		for (auto voice : _voices)
		{
			if (!voice->isPlaying())
			{
				return voice;
			}
		}
		return nullptr;
	}

	std::shared_ptr<Voice> Synthesiser::getVoicePlayingNote(const int midiNoteNumber)
	{
		for (auto voice : _voices)
		{
			if (voice->isPlayingNote(midiNoteNumber))
			{
				return voice;
			}
		}
		return nullptr;
	}
}