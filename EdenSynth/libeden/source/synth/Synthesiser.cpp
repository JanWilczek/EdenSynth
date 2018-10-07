/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include <algorithm>
#include "synth/Synthesiser.h"

#include "eden/AudioBuffer.h"
#include "eden/MidiBuffer.h"
#include "eden/MidiMessage.h"
#include "utility/EdenAssert.h"

namespace eden::synth
{
	Synthesiser::Synthesiser(double sampleRate)
		: _sampleRate(sampleRate)
	{
		constexpr unsigned VOICES_TO_ADD = 16;
		addVoices(VOICES_TO_ADD);
	}

	void Synthesiser::processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int numSamples)
	{
		processBlock(bufferToFill, midiBuffer, 0, numSamples);
	}

	void Synthesiser::processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int startSample, int numSamples)
	{
		bufferToFill.fill(0);

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
		for (auto& voice : _voices)
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
		for (auto& voice : _voices)
		{
			voice->renderBlock(outputBuffer, startSample, samplesToProcess);
		}
	}

	void Synthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
	{
		EDEN_ASSERT(!getVoicePlayingNote(midiNoteNumber));

		auto voice = getFreeVoice();

		EDEN_ASSERT(voice);

		if (voice)
		{
			voice->startNote(midiNoteNumber, velocity, 0);	// TODO: handle pitch wheel
		}

	}
	
	void Synthesiser::noteOff(const int midiChannel, const int midiNoteNumber, const float velocity)
	{
		auto voice = getVoicePlayingNote(midiNoteNumber);

		EDEN_ASSERT(voice);

		if (voice)
		{
			voice->stopNote(velocity, true);
		}
	}

	void Synthesiser::addVoices(unsigned numVoicesToAdd)
	{
		for (unsigned i = 0; i < numVoicesToAdd; ++i)
		{
			_voices.emplace_back(std::make_unique<Voice>(_sampleRate));
		}
	}

	Voice* Synthesiser::getFreeVoice() const
	{
		const auto result = std::find_if_not(_voices.begin(), _voices.end(), [](std::unique_ptr<Voice>& voice) { return voice->isPlaying(); });
		return result != _voices.end() ? result->get() : nullptr;
	}

	Voice* Synthesiser::getVoicePlayingNote(const int midiNoteNumber) const
	{
		const auto result = std::find_if(_voices.begin(), _voices.end(), [&midiNoteNumber](std::unique_ptr<Voice>& voice) { return voice->isPlayingNote(midiNoteNumber); });
		return result != _voices.end() ? result->get() : nullptr;
	}
}
