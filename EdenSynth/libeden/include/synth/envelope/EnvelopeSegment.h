#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include <chrono>
#include <memory>
#include "eden/SampleType.h" 
#include "synth/envelope/IEnvelopeGain.h"

using namespace std::chrono_literals;

namespace eden::synth::envelope
{
	class EnvelopeSegment
	{
	public:
		explicit EnvelopeSegment(double sampleRate, std::unique_ptr<IEnvelopeGain> envelopeGain, std::chrono::milliseconds duration = 0ms, SampleType initialLevel = 0, SampleType finalLevel = 0);
		virtual ~EnvelopeSegment() = 0;

		virtual void applyAndUpdateGain(SampleType& currentLevel);
		virtual bool hasEnded(SampleType currentLevel) = 0;
		virtual void setSampleRate(double sampleRate);

	protected:
		double _sampleRate;
		std::unique_ptr<IEnvelopeGain> _envelopeGain;
		std::chrono::milliseconds _duration;
		SampleType _initialLevel;
		SampleType _finalLevel;
	};
}
