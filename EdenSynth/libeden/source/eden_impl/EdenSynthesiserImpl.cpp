#include "eden_impl/EdenSynthesiserImpl.h"
#include "eden/AudioBuffer.h"

namespace eden
{
	EdenSynthesiserImpl::EdenSynthesiserImpl()
		: _synthesiser(new synth::Synthesiser())
	{}

	void EdenSynthesiserImpl::processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer)
	{
		_synthesiser->processBlock(bufferToFill, midiBuffer, bufferToFill.getNumSamples());
	}

	void EdenSynthesiserImpl::setSampleRate(double sampleRate)
	{
		_sampleRate = sampleRate;
		_synthesiser->setSampleRate(sampleRate);
	}

	double EdenSynthesiserImpl::sampleRate() const noexcept
	{
		return _sampleRate;
	}


}