/// 
/// \author Jan Wilczek
/// \date 29.08.2018
/// 
#include "eden_impl/EdenSynthesiserImpl.h"
#include "eden/AudioBuffer.h"
#include "eden/EnvelopeParameters.h"

namespace eden
{
	EdenSynthesiserImpl::EdenSynthesiserImpl()
		: _settings()
		, _synthesiser(_settings, _sampleRate)
	{
	}

	void EdenSynthesiserImpl::processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer)
	{
		_synthesiser.processBlock(bufferToFill, midiBuffer, bufferToFill.getNumSamples());
	}

	void EdenSynthesiserImpl::setSampleRate(double sampleRate)
	{
		if (sampleRate != _sampleRate)
		{
			_sampleRate = sampleRate;
			_synthesiser.setSampleRate(sampleRate);
		}
	}

	double EdenSynthesiserImpl::sampleRate() const noexcept
	{
		return _sampleRate;
	}

	void EdenSynthesiserImpl::setBlockLength(int samplesPerBlock)
	{
		_synthesiser.setBlockLength(samplesPerBlock);
	}

	void EdenSynthesiserImpl::setWaveTable(std::vector<SampleType> waveTable)
	{
		_synthesiser.setWaveTable(waveTable);
	}

	void EdenSynthesiserImpl::setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters)
	{
		//_settings.setEnvelopeParameters(envelopeParameters);
	}

	void EdenSynthesiserImpl::setCutoff(float cutoff)
	{
		_settings.setCutoff(cutoff);
	}

	void EdenSynthesiserImpl::setResonance(float resonance)
	{
		_settings.setResonance(resonance);
	}
}
