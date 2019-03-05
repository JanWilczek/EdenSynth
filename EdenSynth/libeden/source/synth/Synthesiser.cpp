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
#include "synth/envelope/EnvelopeFactory.h"
#include "settings/Settings.h"

namespace eden::synth
{
	Synthesiser::Synthesiser(settings::Settings& settings)
		: _voiceRenderer(std::make_unique<AsynchronousVoiceRenderer>())
		, _mixer(NB_VOICES)
	{
		addVoices(settings, NB_VOICES);
	}

	void Synthesiser::processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int numSamples)
	{
		processBlock(bufferToFill, midiBuffer, 0, numSamples);
	}

	void Synthesiser::processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int startSample, int numSamples)
	{
		const auto firstSampleIndex = startSample;
		const auto blockLength = numSamples;

		// 1. Empty the buffer - it may contain garbage.
		bufferToFill.fillFromTo(0, startSample, startSample + blockLength);
		
		auto midiIterator = midiBuffer.begin();

		while (numSamples > 0)
		{
			// 2. Calculate the number of samples to render.
			const auto samplesToNextMidiMessage = midiIterator != midiBuffer.end() ? midiIterator->sampleTimeStamp - startSample : numSamples;
			const auto samplesToRender = std::min(numSamples, samplesToNextMidiMessage);

			// 3. Render voices to the end or to the next MIDI message.
			renderVoices(bufferToFill, startSample, samplesToRender);

			// 4. Handle MIDI message if there is any.
			if (midiIterator != midiBuffer.end())
			{
				handleMidiMessage(midiIterator->message);
				++midiIterator;
			}

			// 5. Advance in buffer.
			startSample += samplesToRender;
			numSamples -= samplesToRender;
		}

		// 6. Handle all remaining MIDI messages.
		while (midiIterator != midiBuffer.end())
		{
			handleMidiMessage(midiIterator->message);
			++midiIterator;
		}

		// 7. Clear the MIDI buffer, so that there is no MIDI output.
		midiBuffer.clear();

		// 8. Apply global volume.
		applyVolume(bufferToFill, firstSampleIndex, blockLength);
	}
	
	void Synthesiser::setBlockLength(unsigned samplesPerBlock)
	{
		if (samplesPerBlock != _blockLength)
		{
			_blockLength = samplesPerBlock;
			for (auto& voice : _voices)
			{
				voice->setBlockLength(_blockLength);
			}
		}
	}

	void Synthesiser::setVolume(float volume)
	{
		_volume = volume;
	}

	Synthesiser::IVoiceRenderer::~IVoiceRenderer() {}

	void Synthesiser::SynchronousVoiceRenderer::renderVoices(Synthesiser& synthesiser, AudioBuffer& outputBuffer, int startSample, int samplesToProcess)
	{
		for (auto i = 0u; i < NB_VOICES; ++i)
		{
			synthesiser._mixer[i] = synthesiser._voices[i]->renderBlock(outputBuffer, startSample, samplesToProcess);
		}
	}

	void Synthesiser::AsynchronousVoiceRenderer::renderVoices(Synthesiser& synthesiser, AudioBuffer& outputBuffer, int startSample, int samplesToProcess)
	{
		std::atomic<unsigned> voicesRendered = 0u;

		//std::vector<const float*> renderedVoices(NB_VOICES);

		//for (auto& voice : synthesiser._voices)
		for (auto i = 0u; i < NB_VOICES; ++i)
		{
			_threadPool.submit([&, i]
			{
				synthesiser._mixer[i] = synthesiser._voices[i]->renderBlock(outputBuffer, startSample, samplesToProcess);

				++voicesRendered;
			});
		}

		// wait for all the voices to render
		while (voicesRendered < NB_VOICES)
		{
			std::this_thread::yield();
		}

		synthesiser._mixer.mixTo(outputBuffer, startSample, samplesToProcess);
		/*outputBuffer.forEachChannel([&renderedVoices, &startSample, &samplesToMix=samplesToProcess](float* channel)
		{
			for (auto renderedVoice : renderedVoices)
			{
				if (renderedVoice)
				{
					for (auto sample = 0; sample < samplesToMix; ++sample)
					{
						channel[sample + startSample] += renderedVoice[sample];
					}
				}
			}
		});*/
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
		case MidiMessage::MidiMessageType::PitchBendChange:
			pitchBendChange(channel, midiMessage.getPitchWheelPosition());
			break;
		default:
			break;
		}
	}

	void Synthesiser::renderVoices(AudioBuffer& outputBuffer, int startSample, int samplesToProcess)
	{
		EDEN_ASSERT(samplesToProcess >= 0);

		_voiceRenderer->renderVoices(*this, outputBuffer, startSample, samplesToProcess);
	}

	void Synthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
	{
		// The note may already be playing.
		auto voice = getVoicePlayingNote(midiNoteNumber);
		if (voice)
		{
			voice->startNote(midiNoteNumber, velocity);
		}
		else
		{
			voice = getFreeVoice();
			if (voice)
			{
				voice->startNote(midiNoteNumber, velocity);
			}
		}
	}
	
	void Synthesiser::noteOff(const int midiChannel, const int midiNoteNumber, const float velocity)
	{
		auto voice = getVoicePlayingNote(midiNoteNumber);

		if (voice)
		{
			voice->stopNote(velocity);
		}
	}

	void Synthesiser::pitchBendChange(const int midiChannel, const int pitchBendValue)
	{
		for (auto& voice : _voices)
		{
			voice->setPitchBend(pitchBendValue);
		}
	}

	void Synthesiser::addVoices(settings::Settings& settings, unsigned numVoicesToAdd)
	{
		for (unsigned i = 0u; i < numVoicesToAdd; ++i)
		{
			_voices.emplace_back(std::make_unique<Voice>(settings));
		}
	}

	Voice* Synthesiser::getFreeVoice()
	{
		const auto result = std::find_if_not(_voices.begin(), _voices.end(), [](std::unique_ptr<Voice>& voice) { return voice->isPlaying(); });
		return result != _voices.end() ? result->get() : nullptr;
	}

	Voice* Synthesiser::getVoicePlayingNote(const int midiNoteNumber)
	{
		const auto result = std::find_if(_voices.begin(), _voices.end(), [&midiNoteNumber](std::unique_ptr<Voice>& voice) { return voice->isPlayingNote(midiNoteNumber); });
		return result != _voices.end() ? result->get() : nullptr;
	}

	void Synthesiser::applyVolume(AudioBuffer& bufferToFill, int startSample, int numSamples)
	{
		bufferToFill.forEachChannel([&](float* channel)
		{
			for (auto sample = startSample; sample < startSample + numSamples; ++sample)
			{
				channel[sample] *= _volume;
			}
		});
	}
}
