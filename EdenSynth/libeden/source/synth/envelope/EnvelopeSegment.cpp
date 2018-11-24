/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	EnvelopeSegment::EnvelopeSegment(double sampleRate, std::unique_ptr<ISegmentGain> envelopeGain, std::chrono::milliseconds duration, float initialLevel, float finalLevel)
		: _sampleRate(sampleRate)
		, _envelopeGain(std::move(envelopeGain))
		, _duration(duration)
		, _initialLevel(initialLevel)
		, _finalLevel(finalLevel)
	{
		calculateGain();
	}

	EnvelopeSegment::~EnvelopeSegment()
	{
	}

	void EnvelopeSegment::applyAndUpdateGain(float& currentLevel)
	{
		_envelopeGain->applyAndUpdateGain(currentLevel);
	}

	void EnvelopeSegment::setDuration(std::chrono::milliseconds duration)
	{
		_duration = duration;
		calculateGain();
	}

	void EnvelopeSegment::setGainCurve(std::unique_ptr<ISegmentGain> envelopeGain)
	{
		_envelopeGain = std::move(envelopeGain);
		calculateGain();
	}

	void EnvelopeSegment::setInitialLevel(float initialLevel)
	{
		_initialLevel = initialLevel;
		calculateGain();
	}

	void EnvelopeSegment::setFinalLevel(float finalLevel)
	{
		_finalLevel = finalLevel;
		calculateGain();
	}

	void EnvelopeSegment::setSampleRate(double sampleRate)
	{
		_sampleRate = sampleRate;
		calculateGain();
	}

	void EnvelopeSegment::calculateGain()
	{
		_envelopeGain->calculateGain(_sampleRate, _duration, _initialLevel, _finalLevel);
	}
}