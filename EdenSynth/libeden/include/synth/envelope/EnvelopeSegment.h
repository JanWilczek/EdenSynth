#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include <chrono>
#include <memory>
#include "eden/SampleType.h" 
#include "synth/envelope/ISegmentGain.h"

using namespace std::chrono_literals;

namespace eden::synth::envelope
{
	/// <summary>
	/// Abstract class representing envelope segment, i.e. time segment with gain given with a function of time.
	/// Subclasses may represent attack or decay for example.
	/// It's gain curve is based on passed ISegmentGain's subclass and the duration of the gain change from initial to the final level.
	/// </summary>
	class EnvelopeSegment
	{
	public:
		EnvelopeSegment(double sampleRate, std::unique_ptr<ISegmentGain> envelopeGain, std::chrono::milliseconds duration = 0ms, SampleType initialLevel = 0, SampleType finalLevel = 0);
		virtual ~EnvelopeSegment() = 0;

		/// <summary>
		/// Changes the current envelope level.
		/// </summary>
		/// <param name="currentLevel"></param>
		virtual void applyAndUpdateGain(SampleType& currentLevel);

		/// <returns>true if current value of envelope means that current segment has ended, false otherwise</returns>
		virtual bool hasEnded(SampleType currentLevel) = 0;

		virtual void setDuration(std::chrono::milliseconds duration);

		virtual void setGainCurve(std::unique_ptr<ISegmentGain> envelopeGain);

		virtual void setSampleRate(double sampleRate);

	private:
		void calculateGain();

	protected:
		double _sampleRate;
		std::unique_ptr<ISegmentGain> _envelopeGain;
		std::chrono::milliseconds _duration;
		SampleType _initialLevel;
		SampleType _finalLevel;
	};
}
