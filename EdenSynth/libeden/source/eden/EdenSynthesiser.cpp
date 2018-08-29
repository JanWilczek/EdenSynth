#include "eden/EdenSynthesiser.h"
#include "eden_impl/EdenSynthesiserImpl.h"
#include "eden/AudioBuffer.h"
#include "eden/MidiBuffer.h"

namespace eden
{
	EdenSynthesiser::EdenSynthesiser()
		: _impl(new EdenSynthesiserImpl())
	{}

	void EdenSynthesiser::processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer)
	{
		_impl->processInputBlock(bufferToFill, midiBuffer);
	}

}