/// 
/// \author Jan Wilczek
/// \date 29.08.2018
/// 
#include "eden_impl/EdenSynthesiserImpl.h"
#include "eden/AudioBuffer.h"

namespace eden
{
	EdenSynthesiserImpl::EdenSynthesiserImpl()
		: _synthesiser(std::make_unique<synth::Synthesiser>(_sampleRate))
	{
	}

	void EdenSynthesiserImpl::processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer)
	{
		_synthesiser->processBlock(bufferToFill, midiBuffer, bufferToFill.getNumSamples());
	}

	void EdenSynthesiserImpl::setSampleRate(double sampleRate)
	{
		if (sampleRate != _sampleRate)
		{
			_sampleRate = sampleRate;
			_synthesiser->setSampleRate(sampleRate);
		}
	}

	double EdenSynthesiserImpl::sampleRate() const noexcept
	{
		return _sampleRate;
	}

	void EdenSynthesiserImpl::setBlockLength(int samplesPerBlock)
	{
		_synthesiser->setBlockLength(samplesPerBlock);
	}

	void EdenSynthesiserImpl::setEnvelope(std::unique_ptr<synth::envelope::Envelope> envelope)
	{
		_synthesiser->setEnvelope(std::move(envelope));
	}
}
