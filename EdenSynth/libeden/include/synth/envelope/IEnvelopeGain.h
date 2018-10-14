#pragma once
/// 
/// \author Jan Wilczek
/// \date 14.10.18
/// 
#include <chrono>
#include "eden/SampleType.h"

namespace eden::synth::envelope
{
	class IEnvelopeGain
	{
	public:
		virtual ~IEnvelopeGain() = 0;

		virtual void calculateGain(double sampleRate, std::chrono::milliseconds duration, SampleType initialLevel, SampleType finalLevel) = 0;
		virtual void applyAndUpdateGain(SampleType& currentLevel) = 0;
	};

	inline IEnvelopeGain::~IEnvelopeGain()
	{}
}
