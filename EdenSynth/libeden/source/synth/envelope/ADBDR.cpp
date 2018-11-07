/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/ADBDR.h"
#include "eden/EnvelopeParameters.h"
#include "synth/envelope/SegmentGainFactory.h"

namespace eden::synth::envelope
{
	ADBDR::ADBDR(double sampleRate, ADBDRParameters parameters)
		: Envelope()
		, _breakLevel(parameters.breakLevel)
		, _attack(sampleRate, SegmentGainFactory::createSegmentGain(parameters.attackCurve), parameters.attackTime, 0.0, 1.0)
		, _decay1(sampleRate, SegmentGainFactory::createSegmentGain(parameters.decay1Curve), parameters.decay1Time, 1.0, _breakLevel)
		, _decay2(sampleRate, SegmentGainFactory::createSegmentGain(parameters.decay2Curve), parameters.decay2Time, _breakLevel, 0.0)
		, _release(sampleRate, SegmentGainFactory::createSegmentGain(parameters.releaseCurve), parameters.releaseTime, _breakLevel, 0.0)
		, _silence()
	{
		_segments.push_back(&_attack);
		_segments.push_back(&_decay1);
		_segments.push_back(&_decay2);
		_segments.push_back(&_release);
		_segments.push_back(&_silence);
	}

	void ADBDR::keyOn()
	{
		switchToSegment(static_cast<int>(ADBDRSegments::Attack));;
	}

	void ADBDR::keyOff()
	{
		switchToSegment(static_cast<int>(ADBDRSegments::Release));;
	}

	void ADBDR::setSegmentTime(ADBDRSegments segment, std::chrono::milliseconds time)
	{
		_segments[static_cast<int>(segment)]->setDuration(time);
	}

	void ADBDR::setSegmentCurve(ADBDRSegments segment, EnvelopeSegmentCurve curve)
	{
		_segments[static_cast<int>(segment)]->setGainCurve(SegmentGainFactory::createSegmentGain(curve));
	}

	void ADBDR::checkForEnd(unsigned currentSampleIndex)
	{
		if (_currentSegment >= _segments.size() - 1)
		{
			_currentLevel = 0.0;
			_onEnvelopeEndedCallback(currentSampleIndex);
		}
	}
}
