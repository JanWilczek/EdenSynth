/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "synth/envelope/Envelope.h"

namespace eden::synth::envelope
{
	void Envelope::apply(SampleType* channel, int startSample, int samplesToApply)
	{
		for (auto sample = startSample; sample < startSample + samplesToApply; ++sample)
		{
			channel[sample] *= _currentGain;
			updateGain();

			if (_segments[_currentSegment].hasEnded(_currentGain))
			{
				switchToNextSegment();
			}
		}
	}

	void Envelope::keyOn()
	{
		_currentSegment = 0;
	}

	void Envelope::keyOff()
	{
		_currentSegment = _segments.size() - 1;
	}

	void Envelope::setSampleRate(double sampleRate)
	{
		for (auto& segment : _segments)
		{
			segment.setSampleRate(sampleRate);
		}
	}

	void Envelope::setOnEnvelopeEndedCallback(OnEnvelopeEnded callback)
	{
		_onEnvelopeEndedCallback = callback;
	}

	void Envelope::switchToNextSegment()
	{
		_segments[_currentSegment].reset();
		_currentSegment++;

		// check if envelope has ended
		if (_currentSegment >= _segments.size())
		{
			_onEnvelopeEndedCallback();
		}
	}

	void Envelope::updateGain()
	{
		_currentGain += _segments[_currentSegment].updateAndReturnPerSampleGain();
	}
}