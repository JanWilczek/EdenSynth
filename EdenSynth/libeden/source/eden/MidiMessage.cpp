/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include <cmath>

#include "eden/MidiMessage.h"

namespace eden
{
	float MidiMessage::getMidiNoteInHertz(int midiNoteNumber, const float frequencyOfA) noexcept
	{
		return frequencyOfA * std::pow(2.f, (midiNoteNumber - 69) / 12.f);
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

	int MidiMessage::getPitchWheelPosition()
	{
		return _pitchWheelPosition;
	}

	void MidiMessage::setPitchWheelPosition(int position)
	{
		_pitchWheelPosition = position;
	}
}
