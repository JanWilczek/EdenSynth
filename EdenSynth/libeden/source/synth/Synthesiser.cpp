#include "synth/Synthesiser.h"

#include "eden/AudioBuffer.h"
#include "eden/MidiBuffer.h"
#include "eden/MidiMessage.h"
#include "synth/Voice.h"

namespace eden::synth
{
	void Synthesiser::processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int numSamples)
	{
		processBlock(bufferToFill, midiBuffer, 0, numSamples);
	}

	void Synthesiser::processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int startSample, int numSamples)
	{
		int samplesToProcess;
		auto midiIterator = midiBuffer.begin();

		while (numSamples > 0)
		{
			// 1. Proceed with voice rendering if there are no midi events.
			if (midiIterator == midiBuffer.end())
			{
				renderVoices(bufferToFill, startSample, samplesToProcess);

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

			// 3. Redner samples till the next midi message.
			renderVoices(bufferToFill, startSample, samplesToNextMidiMessage);

			handleMidiMessage(midiIterator->message);

			// 4. Advance in buffer.
			startSample += samplesToNextMidiMessage;
			numSamples -= samplesToNextMidiMessage;
		}

		while (midiIterator != midiBuffer.end())
		{
			handleMidiMessage(midiIterator->message);
		}

		midiBuffer.clear();
	}

	void Synthesiser::handleMidiMessage(MidiMessage& midiMessage)
	{
		const int channel = midiMessage.getChannel();
		const MidiMessage::MidiMessageType messageType = midiMessage.getType();

		switch(messageType)
		{
		case MidiMessage::MidiMessageType::NoteOn:
			noteOn(channel, midiMessage.getNoteNumber(), midiMessage.getFloatVelocity());
			break;
		case MidiMessage::MidiMessageType::NoteOff:
			noteOff(channel, midiMessage.getNoteNumber(), midiMessage.getFloatVelocity());
			break;
		default:
			break;
		}
	}

	void Synthesiser::renderVoices(AudioBuffer& outputBuffer, int startSample, int samplesToProcess)
	{
		for (auto& voice : _voices)
		{
			voice->renderBlock(outputBuffer, startSample, samplesToProcess);
		}
	}

	void Synthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
	{

	}
	
	void Synthesiser::noteOff(const int midiChannel, const int midiNoteNumber, const float velocity)
	{
		
	}

}