/// 
/// \author Jan Wilczek
/// \date 29.08.2018
/// 
#include "eden/EdenSynthesiser.h"
#include "eden/AudioBuffer.h"
#include "eden/MidiBuffer.h"
#include "eden/EnvelopeParameters.h"

namespace eden
{
	EdenSynthesiser::EdenSynthesiser()
		: _impl(std::make_unique<EdenSynthesiserImpl>())
	{}

	void EdenSynthesiser::processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer)
	{
		_impl->processInputBlock(bufferToFill, midiBuffer);
	}

	void EdenSynthesiser::setSampleRate(double sampleRate)
	{
		_impl->setSampleRate(sampleRate);
	}

	double EdenSynthesiser::sampleRate() const noexcept
	{
		return _impl->sampleRate();
	}

	void EdenSynthesiser::setBlockLength(int samplesPerBlock)
	{
		_impl->setBlockLength(samplesPerBlock);
	}

	void EdenSynthesiser::setWaveTable(std::vector<SampleType> waveTable)
	{
		_impl->setWaveTable(waveTable);
	}

	void EdenSynthesiser::setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters)
	{
		_impl->setEnvelopeParameters(envelopeParameters);
	}

	void EdenSynthesiser::setCutoff(float cutoff)
	{
		_impl->setCutoff(cutoff);
	}

	void EdenSynthesiser::setResonance(float resonance)
	{
		_impl->setResonance(resonance);
	}
}
