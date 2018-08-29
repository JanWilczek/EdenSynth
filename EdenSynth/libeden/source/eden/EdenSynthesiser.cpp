#include "eden/EdenSynthesiser.h"
#include "eden/AudioBuffer.h"
#include "eden/MidiBuffer.h"

namespace eden
{
	EdenSynthesiser::EdenSynthesiser()
		: _impl(std::make_unique<EdenSynthesiserImpl>())
	{}

	void EdenSynthesiser::processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer)
	{
		_impl->processInputBlock(bufferToFill, midiBuffer);
	}
}