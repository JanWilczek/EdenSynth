#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include <chrono>
#include "eden/SampleType.h" 

namespace eden::synth::envelope
{
	class EnvelopeSegment
	{
	public:
		explicit EnvelopeSegment(double sampleRate, SampleType initialLevel = 0, SampleType finalLevel = 0, std::chrono::milliseconds duration = std::chrono::milliseconds(0));
		virtual ~EnvelopeSegment() = 0;

		virtual SampleType updateAndReturnPerSampleGain();
		virtual bool hasEnded(SampleType currentGain) = 0;
		virtual void reset() = 0;
		virtual void setSampleRate(double sampleRate);

	private:
		double _sampleRate;
		SampleType _gainPerSample = 0;
		SampleType _initialLevel;
		SampleType _finalLevel;
		std::chrono::milliseconds _duration;
	};
}
