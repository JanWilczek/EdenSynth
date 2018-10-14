#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include <vector>
#include <functional>
#include "eden/SampleType.h"

namespace eden::synth::envelope
{
	class EnvelopeSegment;

	class Envelope
	{
	public:
		typedef std::function<void()> OnEnvelopeEnded;

		virtual ~Envelope() = 0;
		virtual void apply(SampleType* channel, int startSample, int samplesToApply);
		virtual void keyOn() = 0;
		virtual void keyOff() = 0;
		/// <summary>
		/// Checks if the envelope has ended and applies appropriate actions if it did.
		/// </summary>
		virtual void checkForEnd() = 0;
		virtual void setSampleRate(double sampleRate);

		void setOnEnvelopeEndedCallback(OnEnvelopeEnded callback);

	protected:
		void switchToSegment(size_t segment);
		void updateGain();

		SampleType _currentGain = 0.0;
		std::vector<EnvelopeSegment*> _segments;
		size_t _currentSegment = 0;
		OnEnvelopeEnded _onEnvelopeEndedCallback = [](){};
	};
}
