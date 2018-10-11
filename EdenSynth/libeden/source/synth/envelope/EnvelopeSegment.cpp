/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	EnvelopeSegment::EnvelopeSegment(double sampleRate, SampleType initialLevel, SampleType finalLevel, std::chrono::milliseconds duration)
		: _sampleRate(sampleRate)
		, _initialLevel(initialLevel)
		, _finalLevel(finalLevel)
		, _duration(duration)
	{
	}

	SampleType EnvelopeSegment::updateAndReturnPerSampleGain()
	{
		return _gainPerSample;
	}

	void EnvelopeSegment::setSampleRate(double sampleRate)
	{
		_sampleRate = sampleRate;
	}
}