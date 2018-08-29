#pragma once

namespace eden
{
	class MidiMessage;

	class MidiBuffer
	{
	public:
		unsigned int getNumEvents() const noexcept;
		void addEvent(const MidiMessage& messageToAdd, int sampleNumber);
		void clear();
	};
}