#pragma once
#include <vector>

#include "eden/MidiMessage.h"

namespace eden
{
	class MidiBuffer
	{
	public:
		struct MidiStampedMessage
		{
			MidiMessage message;
			int sampleTimeStamp;
		};
		typedef std::vector<MidiStampedMessage>::iterator MidiIterator;

		MidiBuffer() = default;

		unsigned int getNumEvents() const noexcept;
		void addEvent(const MidiMessage& messageToAdd, int messageSamplePosition);
		void clear();

		MidiIterator begin();
		MidiIterator end();

	private:
		std::vector<MidiStampedMessage> _messages;
	};
}
