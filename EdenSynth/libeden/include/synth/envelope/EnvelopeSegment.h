#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include "eden/SampleType.h" 
#include <chrono>

using namespace std::chrono_literals;

namespace eden::synth::envelope
{
	class EnvelopeSegment
	{
	public:
		explicit EnvelopeSegment(double sampleRate, SampleType initialLevel = 0, SampleType finalLevel = 0, std::chrono::milliseconds duration = 0ms);

		void apply(SampleType& sample);
		bool hasEnded();
		void reset();

	private:
		SampleType _currentGain = 0;
	};
}
