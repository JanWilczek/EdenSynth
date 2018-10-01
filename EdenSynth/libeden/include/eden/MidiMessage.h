#pragma once
/// 
/// \author Jan Wilczek
/// \date 27.08.2018
/// 

namespace eden
{
	/// <summary>
	/// Single MIDI message.
	/// </summary>
	class MidiMessage
	{
	public:
		/// <summary>
		/// Type of the message.
		/// </summary>
		enum class MidiMessageType
		{
			NoteOn,
			NoteOff,
			Aftertouch,
			ControlChange,
			PitchBendChange
		};

		/// <summary>
		/// Converts MIDI note number to frequency expressed in Hz.
		/// </summary>
		/// <param name="midiNoteNumber"></param>
		/// <param name="frequencyOfA"></param>
		/// <returns>calculated frequency in Hz</returns>
		static double getMidiNoteInHertz(int midiNoteNumber, const double frequencyOfA = 440.0) noexcept;

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