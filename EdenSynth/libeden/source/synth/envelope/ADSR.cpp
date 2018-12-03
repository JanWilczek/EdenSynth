/// 
/// \author Jan Wilczek
/// \date 27.11.2018
/// 
#include "synth/envelope/ADSR.h"
#include "eden/EnvelopeParameters.h"
#include "synth/envelope/SegmentGainFactory.h"

namespace eden::synth::envelope
{
	ADSR::ADSR(float sampleRate, ADSRParameters parameters)
		: _attack(sampleRate, SegmentGainFactory::createSegmentGain(parameters.attackCurve), parameters.attackTime, 0.0f, 1.0f)
		, _decay(sampleRate, SegmentGainFactory::createSegmentGain(parameters.decayCurve), parameters.decayTime, 1.0f, parameters.sustainLevel)
		, _release(sampleRate, SegmentGainFactory::createSegmentGain(parameters.releaseCurve), parameters.releaseTime, parameters.sustainLevel, 0.f)
	{
		_segments.insert(_segments.begin() + static_cast<int>(ADSRSegments::Attack), &_attack);
		_segments.insert(_segments.begin() + static_cast<int>(ADSRSegments::Decay), &_decay);
		_segments.insert(_segments.begin() + static_cast<int>(ADSRSegments::Sustain), &_sustain);
		_segments.insert(_segments.begin() + static_cast<int>(ADSRSegments::Release), &_release);
		_segments.insert(_segments.begin() + static_cast<int>(ADSRSegments::Silence), &_silence);
	}

	void ADSR::keyOn()
	{
		switchToSegment(static_cast<int>(ADSRSegments::Attack));
	}

	void ADSR::keyOff()
	{
		switchToSegment(static_cast<int>(ADSRSegments::Release));
	}

	void ADSR::setSegmentTime(ADSRSegments segment, std::chrono::milliseconds time)
	{
		_segments[static_cast<int>(segment)]->setDuration(time);
	}

	void ADSR::setSegmentCurve(ADSRSegments segment, EnvelopeSegmentCurve curve)
	{
		_segments[static_cast<int>(segment)]->setGainCurve(SegmentGainFactory::createSegmentGain(curve));
	}

	void ADSR::setSustainLevel(float sustainLevel)
	{
		_decay.setFinalLevel(sustainLevel);
		_release.setInitialLevel(sustainLevel);
	}

	bool ADSR::hasEnded()
	{
		return _currentSegment >= static_cast<int>(ADSRSegments::Silence);
	}
}
