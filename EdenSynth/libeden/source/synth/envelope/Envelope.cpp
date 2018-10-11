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
			_segments[_currentSegment].apply(channel[sample]);

			if (_segments[_currentSegment].hasEnded())
			{
				switchToNextSegment();
			}
		}
	}

	void Envelope::switchToNextSegment()
	{
		_segments[_currentSegment].reset();
		_currentSegment = (_currentSegment + 1) % _segments.size();
	}
}