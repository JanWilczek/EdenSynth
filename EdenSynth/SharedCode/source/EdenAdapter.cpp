/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include "EdenAdapter.h"
#include "eden/MidiBuffer.h"
#include "eden/EdenSynthesiser.h"

namespace eden_vst
{
	eden::MidiBuffer EdenAdapter::convertToEdenMidi(const juce::MidiBuffer& juceMidiBuffer)
	{
		eden::MidiBuffer edenMidiBuffer;

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

		return edenMidiBuffer;
	}

	void EdenAdapter::addEdenParameters(const eden::EdenSynthesiser& edenSynthesiser, AudioProcessorValueTreeState& pluginParameters)
	{
		pluginParameters.createAndAddParameter("filter.cutoff", "Cutoff", String(), NormalisableRange<float>(0.f, 0.5f, 0.01f), 0.49f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("filter.resonance", "Resonance", String(), NormalisableRange<float>(0.5f, 5.0f), 1.f, nullptr, nullptr);
	}

	void EdenAdapter::updateEdenParameters(eden::EdenSynthesiser& edenSynthesiser, const AudioProcessorValueTreeState& pluginParameters)
	{
		edenSynthesiser.setCutoff(pluginParameters.getParameterAsValue("filter.cutoff").getValue());
		edenSynthesiser.setResonance(pluginParameters.getParameterAsValue("filter.resonance").getValue());
	}
}
