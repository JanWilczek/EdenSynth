/// 
/// \author Jan Wilczek
/// \date 12.10.18
/// 
#include "synth/envelope/ADBDR.h"
#include "synth/envelope/ExponentialGain.h"

namespace eden::synth::envelope
{
	ADBDR::ADBDR(double sampleRate, ms attackTime, ms decay1Time, ms decay2Time, ms releaseTime, SampleType breakLevel)
		: Envelope()
		, _breakLevel(breakLevel)
		, _attack(sampleRate, std::make_unique<ExponentialGain>(), attackTime, 0.0, 1.0)
		, _decay1(sampleRate, std::make_unique<ExponentialGain>(), decay1Time, 1.0, _breakLevel)
		, _decay2(sampleRate, std::make_unique<ExponentialGain>(), decay2Time, _breakLevel, 0.0)
		, _release(sampleRate, std::make_unique<ExponentialGain>(), releaseTime, _breakLevel, 0.0)
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
		switchToSegment(0);
	}

	void ADBDR::keyOff()
	{
		switchToSegment(_segments.size() - 2);
	}

	void ADBDR::checkForEnd()
	{
		if (_currentSegment >= _segments.size() - 1)
		{
			_currentLevel = 0.0;
			_onEnvelopeEndedCallback();
		}
	}
}