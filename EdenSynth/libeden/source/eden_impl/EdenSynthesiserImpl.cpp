#include "eden_impl/EdenSynthesiserImpl.h"

namespace eden
{
	EdenSynthesiserImpl::EdenSynthesiserImpl()
	{}

	void EdenSynthesiserImpl::processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer)
	{
		
	}

	void EdenSynthesiserImpl::setSampleRate(double sampleRate)
	{
		_sampleRate = sampleRate;
	}

	double EdenSynthesiserImpl::sampleRate() const noexcept
	{
		return _sampleRate;
	}


}