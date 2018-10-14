/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	EnvelopeSegment::EnvelopeSegment(double sampleRate, std::unique_ptr<IEnvelopeGain> envelopeGain, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
		: _sampleRate(sampleRate)
		, _envelopeGain(std::move(envelopeGain))
		, _duration(duration)
		, _initialLevel(initialLevel)
		, _finalLevel(finalLevel)
	{
	}

	EnvelopeSegment::~EnvelopeSegment()
	{
	}

	void EnvelopeSegment::applyAndUpdateGain(SampleType& currentLevel)
	{
		_envelopeGain->applyAndUpdateGain(currentLevel);
	}

	void EnvelopeSegment::setSampleRate(double sampleRate)
	{
		_sampleRate = sampleRate;
		_envelopeGain->calculateGain(_sampleRate, _duration, _initialLevel, _finalLevel);
	}
}