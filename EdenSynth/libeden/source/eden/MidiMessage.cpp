#include <cmath>

#include "eden/MidiMessage.h"

namespace eden
{
	double MidiMessage::getMidiNoteInHertz(int midiNoteNumber, const double frequencyOfA) noexcept
	{
		return frequencyOfA * std::pow(2.0, (midiNoteNumber - 69) / 12.0);
	}

	MidiMessage::MidiMessage(int channel, MidiMessageType messageType)
		: _channel(channel)
		, _type(messageType)
	{}

	int MidiMessage::getChannel() const noexcept
	{
		return _channel;
	}

	void MidiMessage::setChannel(int channel)
	{
		_channel = channel;
	}

	MidiMessage::MidiMessageType MidiMessage::getType() const
	{
		return _type;
	}

	void MidiMessage::setType(MidiMessageType type)
	{
		_type = type;
	}

	int MidiMessage::getNoteNumber() const
	{
		return _noteNumber;
	}

	void MidiMessage::setNoteNumber(int noteNumber)
	{
		_noteNumber = noteNumber;
	}

	float MidiMessage::getVelocity() const
	{
		return _velocity;
	}

	void MidiMessage::setVelocity(float velocity)
	{
		_velocity = velocity;
	}
}
