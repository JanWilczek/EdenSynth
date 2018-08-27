#pragma once

namespace eden
{
	class MidiMessage
	{
		enum class MidiMessageType
		{
			NoteOn,
			NoteOff,
			Aftertouch,
			ControlChange,
			PitchBendChange
		};

	};
}