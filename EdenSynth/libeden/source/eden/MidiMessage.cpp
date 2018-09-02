#include "eden/MidiMessage.h"

namespace eden
{
	MidiMessage::MidiMessage(int channel, MidiMessageType messageType)
		: _channel(channel)
		, _type(messageType)
	{}

	int MidiMessage::getChannel() const noexcept
	{
		return _channel;
	}

	MidiMessage::MidiMessageType MidiMessage::getType() const
	{
		return _type;
	}

	int MidiMessage::getNoteNumber() const
	{
		return _noteNumber;
	}

	float MidiMessage::getVelocity() const
	{
		return _velocity;
	}


}