/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/Envelope.h"
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	Envelope::~Envelope()
	{
	}

	void Envelope::apply(SampleType* channel, int startSample, int samplesToApply)
	{
		for (auto sample = startSample; sample < startSample + samplesToApply; ++sample)
		{
			channel[sample] *= _currentGain;
			updateGain();

			if (_segments[_currentSegment]->hasEnded(_currentGain))
			{
				switchToSegment(_currentSegment + 1);
				checkForEnd();
			}
		}
	}

	void Envelope::keyOn()
	{
		switchToSegment(0);
	}

	void Envelope::keyOff()
	{
		switchToSegment(_segments.size() - 1);
	}

	void Envelope::setSampleRate(double sampleRate)
	{
		for (auto& segment : _segments)
		{
			segment->setSampleRate(sampleRate);
		}
	}

	void Envelope::setOnEnvelopeEndedCallback(OnEnvelopeEnded callback)
	{
		_onEnvelopeEndedCallback = callback;
	}

	void Envelope::checkForEnd()
	{
		if (_currentSegment >= _segments.size())
		{
			_currentGain = 0.0;
			_onEnvelopeEndedCallback();
		}
	}

	void Envelope::switchToSegment(size_t segment)
	{
		_segments[_currentSegment]->reset();
		_currentSegment = segment;
	}

	void Envelope::updateGain()
	{
		_currentGain += _segments[_currentSegment]->updateAndReturnPerSampleGain();
	}
}
