#pragma once

namespace eden
{
	class MidiMessage
	{
	public:
		enum class MidiMessageType
		{
			NoteOn,
			NoteOff,
			Aftertouch,
			ControlChange,
			PitchBendChange
		};

		MidiMessage() = default;

		MidiMessage(int channel, MidiMessageType messageType);

		int getChannel() const noexcept;
		void setChannel(int channel);

		MidiMessageType getType() const;
		void setType(MidiMessageType type);

		int getNoteNumber() const;
		void setNoteNumber(int noteNumber);

		float getVelocity() const;
		void setVelocity(float velocity);

	private:
		MidiMessageType _type;
		int _channel;
		int _noteNumber;
		float _velocity;
	};
}