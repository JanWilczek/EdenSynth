/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	EnvelopeSegment::EnvelopeSegment(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel)
		: _sampleRate(sampleRate)
		, _duration(duration)
		, _initialLevel(initialLevel)
		, _finalLevel(finalLevel)
	{
	}

	EnvelopeSegment::~EnvelopeSegment()
	{
	}

	SampleType EnvelopeSegment::updateAndReturnPerSampleGain()
	{
		return _gainPerSample;
	}

	void EnvelopeSegment::setSampleRate(double sampleRate)
	{
		_sampleRate = sampleRate;
		calculatePerSampleGain();
	}

	void EnvelopeSegment::calculatePerSampleGain()
	{
		constexpr double millisecondsInASecond = 1000.0;
		const auto durationInSamples =  (_duration.count() / millisecondsInASecond) * _sampleRate;
		_gainPerSample = (_finalLevel - _initialLevel) / durationInSamples;
	}
}