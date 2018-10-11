#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include <vector>
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	class Envelope
	{
	public:
		virtual ~Envelope() = 0;
		virtual void apply(SampleType* channel, int startSample, int samplesToApply);
		virtual void keyOn();
		virtual void keyOff();
	private:
		void switchToNextSegment();

		std::vector<EnvelopeSegment> _segments;
		size_t _currentSegment = 0;
	};
}