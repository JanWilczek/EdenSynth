#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include <chrono>
#include "eden/SampleType.h" 

using namespace std::chrono_literals;

namespace eden::synth::envelope
{
	class EnvelopeSegment
	{
	public:
		explicit EnvelopeSegment(double sampleRate, std::chrono::milliseconds duration = 0ms, SampleType initialLevel = 0, SampleType finalLevel = 0);
		virtual ~EnvelopeSegment() = 0;

		virtual SampleType updateAndReturnPerSampleGain();
		virtual bool hasEnded(SampleType currentGain) = 0;
		virtual void reset() = 0;
		virtual void setSampleRate(double sampleRate);

	protected:
		virtual void calculatePerSampleGain();

		double _sampleRate;
		std::chrono::milliseconds _duration;
		SampleType _initialLevel;
		SampleType _finalLevel;
		SampleType _gainPerSample = 0;
	};
}
