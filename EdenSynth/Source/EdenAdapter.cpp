/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include "EdenAdapter.h"
#include "eden/MidiBuffer.h"

namespace eden_vst
{
	void EdenAdapter::convertToEdenBuffer(juce::AudioBuffer<eden::AudioBuffer::SampleType>& juceAudioBuffer, eden::AudioBuffer& edenBuffer)
	{
		/*
		1. Check for number of channels
		2. If they are equal copy the content and set length
		3. If juce's bigger 
		 */
	}

	void EdenAdapter::convertToEdenMidi(const juce::MidiBuffer& juceMidiBuffer, eden::MidiBuffer& edenMidiBuffer)
	{
		int midiEventPos;
		MidiMessage m;

		juce::MidiBuffer::Iterator midiIterator(juceMidiBuffer);

		while (midiIterator.getNextEvent(m, midiEventPos))
		{
			if (m.isNoteOn())
			{
				eden::MidiMessage edenMessage(m.getChannel(), eden::MidiMessage::MidiMessageType::NoteOn);
				edenMessage.setVelocity(m.getFloatVelocity());
				edenMessage.setNoteNumber(m.getNoteNumber());
				edenMessage.setPitchWheelPosition(m.getPitchWheelValue());
				edenMidiBuffer.addEvent(std::move(edenMessage), midiEventPos);
			}
			else if (m.isNoteOff())
			{
				eden::MidiMessage edenMessage(m.getChannel(), eden::MidiMessage::MidiMessageType::NoteOff);
				edenMessage.setVelocity(m.getFloatVelocity());
				edenMessage.setNoteNumber(m.getNoteNumber());
				edenMidiBuffer.addEvent(std::move(edenMessage), midiEventPos);
			}
		}
	}
}